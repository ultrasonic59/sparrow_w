#include "ThkLink.h"

#include "thk_bank.h"
#include "thk_proto.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QNetworkDatagram>
#include <QUdpSocket>

#include <algorithm>
#include <cstring>

static_assert(sizeof(ThkHeader) == 20, "ThkHeader");
static_assert(sizeof(ThkWaveInfo) == 16, "ThkWaveInfo");
static_assert(sizeof(ThkTextInfo) == 4, "ThkTextInfo");
static_assert(THK_WAVE_SAMPLES_PER_CHUNK == 692, "chunk");

namespace {

QByteArray packet(quint16 type, quint32 sequence, const QByteArray& payload)
{
    QByteArray frame(int(sizeof(ThkHeader) + payload.size()), 0);
    ThkHeader header = {};
    header.signature = THK_SIGNATURE;
    header.version = THK_VERSION;
    header.type = type;
    header.sequence = sequence;
    header.length = quint16(payload.size());
    std::memcpy(frame.data(), &header, sizeof(header));
    if (!payload.isEmpty())
        std::memcpy(frame.data() + sizeof(header), payload.constData(), size_t(payload.size()));
    thk_seal(reinterpret_cast<uint8_t*>(frame.data()), size_t(frame.size()));
    return frame;
}

bool readHeader(const QByteArray& frame, ThkHeader* header)
{
    if (frame.size() < int(sizeof(ThkHeader)))
        return false;
    if (!thk_check(reinterpret_cast<const uint8_t*>(frame.constData()), size_t(frame.size())))
        return false;
    std::memcpy(header, frame.constData(), sizeof(*header));
    if (header->signature != THK_SIGNATURE || header->version != THK_VERSION)
        return false;
    if (int(sizeof(ThkHeader)) + header->length != frame.size())
        return false;
    return true;
}

} // namespace

ThkStubReceiver::ThkStubReceiver(QObject* parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
{
    connect(m_socket, &QUdpSocket::readyRead, this, &ThkStubReceiver::onReadyRead);
}

bool ThkStubReceiver::listen(quint16 port)
{
    close();
    resetAssembly();
    return m_socket->bind(QHostAddress(QHostAddress::LocalHost), port);
}

void ThkStubReceiver::close()
{
    m_socket->close();
}

bool ThkStubReceiver::isListening() const
{
    return m_socket->state() == QAbstractSocket::BoundState;
}

quint16 ThkStubReceiver::port() const
{
    return m_socket->localPort();
}

void ThkStubReceiver::dropNextWave()
{
    m_dropNext = true;
}

bool ThkStubReceiver::complete() const
{
    return m_bank.complete != 0;
}

const std::vector<std::int16_t>& ThkStubReceiver::samples() const
{
    return m_samples;
}

quint32 ThkStubReceiver::sampleRateHz() const
{
    return m_bank.rate;
}

QByteArray ThkStubReceiver::lastText() const
{
    return m_text;
}

void ThkStubReceiver::resetAssembly()
{
    thk_bank_reset(&m_bank);
    m_samples.clear();
    m_text.clear();
}

void ThkStubReceiver::pullSamples()
{
    m_samples.clear();
    if (m_bank.complete && m_bank.total > 0)
        m_samples.assign(m_bank.samples, m_bank.samples + m_bank.total);
}

uint16_t ThkStubReceiver::acceptWave(const uint8_t* payload, int length)
{
    if (length < 0)
        return THK_ACK_FORMAT;
    const uint16_t status = thk_bank_wave(&m_bank, payload, static_cast<uint16_t>(length));
    pullSamples();
    return status;
}

void ThkStubReceiver::onReadyRead()
{
    while (m_socket->hasPendingDatagrams()) {
        const QNetworkDatagram datagram = m_socket->receiveDatagram();
        const QByteArray frame = datagram.data();
        ThkHeader header = {};
        if (!readHeader(frame, &header))
            continue;
        const uint8_t* payload = reinterpret_cast<const uint8_t*>(frame.constData()) + sizeof(ThkHeader);

        if (header.type == THK_WAVE && m_dropNext) {
            m_dropNext = false;
            continue;
        }

        QByteArray replyPayload;
        quint16 replyType = 0;
        if (header.type == THK_HELLO || header.type == THK_CTRL) {
            ThkStatusBody status = {};
            thk_bank_status(&m_bank, &status);
            replyPayload = QByteArray(reinterpret_cast<const char*>(&status), int(sizeof(status)));
            replyType = THK_STATUS;
        } else if (header.type == THK_WAVE) {
            const uint16_t ackStatus = acceptWave(payload, header.length);
            ThkWaveInfo info = {};
            if (header.length >= int(sizeof(ThkWaveInfo)))
                std::memcpy(&info, payload, sizeof(info));
            ThkAckBody ack = {};
            ack.ack_sequence = header.sequence;
            ack.chunk_index = info.chunk_index;
            ack.status = ackStatus;
            replyPayload = QByteArray(reinterpret_cast<const char*>(&ack), int(sizeof(ack)));
            replyType = THK_ACK;
        } else if (header.type == THK_TEXT) {
            ThkAckBody ack = {};
            ack.ack_sequence = header.sequence;
            if (header.length < int(sizeof(ThkTextInfo))) {
                ack.status = THK_ACK_FORMAT;
            } else {
                m_text = frame.mid(int(sizeof(ThkHeader) + sizeof(ThkTextInfo)), header.length - int(sizeof(ThkTextInfo)));
                ack.status = THK_ACK_OK;
            }
            replyPayload = QByteArray(reinterpret_cast<const char*>(&ack), int(sizeof(ack)));
            replyType = THK_ACK;
        } else {
            continue;
        }

        const QByteArray reply = packet(replyType, header.sequence, replyPayload);
        m_socket->writeDatagram(reply, datagram.senderAddress(), datagram.senderPort());
    }
}

ThkSender::ThkSender(QObject* parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
{
    ensureSocket();
}

bool ThkSender::ensureSocket()
{
    if (m_socket->state() == QAbstractSocket::BoundState)
        return true;
    return m_socket->bind(QHostAddress(QHostAddress::AnyIPv4), quint16(0));
}

bool ThkSender::waitFor(quint32 sequence, quint16 type, int timeoutMs, QByteArray* payload)
{
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < timeoutMs) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
        while (m_socket->hasPendingDatagrams()) {
            const QByteArray frame = m_socket->receiveDatagram().data();
            ThkHeader header = {};
            if (!readHeader(frame, &header))
                continue;
            if (header.type != type || header.sequence != sequence)
                continue;
            if (payload)
                *payload = frame.mid(int(sizeof(ThkHeader)), header.length);
            return true;
        }
    }
    return false;
}

ThkUploadResult ThkSender::upload(const QHostAddress& host, quint16 port, const std::vector<std::int16_t>& samples,
                                  quint32 sampleRateHz)
{
    ThkUploadResult result;
    if (samples.empty() || sampleRateHz == 0) {
        result.error = "Empty buffer";
        return result;
    }
    if (!ensureSocket()) {
        result.error = "Could not open the socket";
        return result;
    }

    const int chunkSamples = THK_WAVE_SAMPLES_PER_CHUNK;
    const int chunkCount = int((samples.size() + chunkSamples - 1) / chunkSamples);
    for (int chunk = 0; chunk < chunkCount; ++chunk) {
        const int begin = chunk * chunkSamples;
        const int count = std::min(chunkSamples, int(samples.size()) - begin);
        ThkWaveInfo info = {};
        info.sample_rate_hz = sampleRateHz;
        info.sample_format = THK_SAMPLE_INT16;
        info.total_samples = quint32(samples.size());
        info.chunk_index = quint16(chunk);
        info.chunk_count = quint16(chunkCount);

        QByteArray payload(int(sizeof(info) + size_t(count) * sizeof(std::int16_t)), 0);
        std::memcpy(payload.data(), &info, sizeof(info));
        std::memcpy(payload.data() + sizeof(info), samples.data() + begin, size_t(count) * sizeof(std::int16_t));

        bool acked = false;
        for (int attempt = 0; attempt < maxAttempts; ++attempt) {
            if (attempt > 0)
                ++result.retries;
            const quint32 sequence = m_sequence++;
            const QByteArray frame = packet(THK_WAVE, sequence, payload);
            if (m_socket->writeDatagram(frame, host, port) != frame.size()) {
                result.error = "Send failed";
                return result;
            }
            QByteArray reply;
            if (!waitFor(sequence, THK_ACK, timeoutMs, &reply) || reply.size() < int(sizeof(ThkAckBody)))
                continue;
            ThkAckBody ack = {};
            std::memcpy(&ack, reply.constData(), sizeof(ack));
            if (ack.ack_sequence == sequence && ack.status == 0 && ack.chunk_index == quint16(chunk)) {
                acked = true;
                break;
            }
        }
        if (!acked) {
            result.error = QString("No acknowledgement, chunk %1").arg(chunk);
            return result;
        }
    }

    result.ok = true;
    return result;
}

bool ThkSender::ping(const QHostAddress& host, quint16 port, ThkStatusBody* status)
{
    if (!ensureSocket())
        return false;
    const quint32 sequence = m_sequence++;
    const QByteArray frame = packet(THK_HELLO, sequence, QByteArray());
    if (m_socket->writeDatagram(frame, host, port) != frame.size())
        return false;
    QByteArray reply;
    if (!waitFor(sequence, THK_STATUS, timeoutMs, &reply) || reply.size() < int(sizeof(ThkStatusBody)))
        return false;
    if (status)
        std::memcpy(status, reply.constData(), sizeof(*status));
    return true;
}

bool ThkSender::command(const QHostAddress& host, quint16 port, quint16 command, quint32 periodUs, ThkStatusBody* status,
                        quint16 reserved)
{
    if (!ensureSocket())
        return false;
    ThkCtrlBody body = {};
    body.command = command;
    body.reserved = reserved;
    body.period_us = periodUs;
    const quint32 sequence = m_sequence++;
    const QByteArray frame = packet(THK_CTRL, sequence,
                                    QByteArray(reinterpret_cast<const char*>(&body), int(sizeof(body))));
    if (m_socket->writeDatagram(frame, host, port) != frame.size())
        return false;
    QByteArray reply;
    if (!waitFor(sequence, THK_STATUS, timeoutMs, &reply) || reply.size() < int(sizeof(ThkStatusBody)))
        return false;
    if (status)
        std::memcpy(status, reply.constData(), sizeof(*status));
    return true;
}

bool ThkSender::sendText(const QHostAddress& host, quint16 port, quint32 baud, const QByteArray& bytes, quint16* ackStatus)
{
    if (ackStatus)
        *ackStatus = 0xffff;
    if (bytes.size() > int(THK_MAX_PAYLOAD) - int(sizeof(ThkTextInfo)))
        return false;
    if (!ensureSocket())
        return false;
    ThkTextInfo info = {};
    info.baud = baud;
    QByteArray payload(int(sizeof(info)) + bytes.size(), 0);
    std::memcpy(payload.data(), &info, sizeof(info));
    if (!bytes.isEmpty())
        std::memcpy(payload.data() + sizeof(info), bytes.constData(), size_t(bytes.size()));
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        const quint32 sequence = m_sequence++;
        const QByteArray frame = packet(THK_TEXT, sequence, payload);
        if (m_socket->writeDatagram(frame, host, port) != frame.size())
            return false;
        QByteArray reply;
        if (!waitFor(sequence, THK_ACK, timeoutMs, &reply) || reply.size() < int(sizeof(ThkAckBody)))
            continue;
        ThkAckBody ack = {};
        std::memcpy(&ack, reply.constData(), sizeof(ack));
        if (ack.ack_sequence != sequence)
            continue;
        if (ackStatus)
            *ackStatus = ack.status;
        if (ack.status == 0)
            return true;
    }
    return false;
}
