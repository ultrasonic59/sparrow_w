#pragma once

#include <QByteArray>
#include <QHostAddress>
#include <QObject>
#include <QString>

#include "thk_bank.h"

#include <cstdint>
#include <vector>

struct ThkStatusBody;

struct ThkUploadResult {
    bool ok = false;
    int retries = 0;
    QString error;
};

class QUdpSocket;

class ThkStubReceiver : public QObject {
    Q_OBJECT

public:
    explicit ThkStubReceiver(QObject* parent = nullptr);

    bool listen(quint16 port);
    void close();
    bool isListening() const;
    quint16 port() const;
    void dropNextWave();

    bool complete() const;
    const std::vector<std::int16_t>& samples() const;
    quint32 sampleRateHz() const;
    QByteArray lastText() const;

private slots:
    void onReadyRead();

private:
    void resetAssembly();
    uint16_t acceptWave(const uint8_t* payload, int length);
    void pullSamples();

    QUdpSocket* m_socket = nullptr;
    bool m_dropNext = false;
    ThkBank m_bank = {};
    std::vector<std::int16_t> m_samples;
    QByteArray m_text;
};

class ThkSender : public QObject {
    Q_OBJECT

public:
    explicit ThkSender(QObject* parent = nullptr);

    ThkUploadResult upload(const QHostAddress& host, quint16 port, const std::vector<std::int16_t>& samples,
                           quint32 sampleRateHz);
    bool ping(const QHostAddress& host, quint16 port, ThkStatusBody* status);
    bool command(const QHostAddress& host, quint16 port, quint16 command, quint32 periodUs, ThkStatusBody* status,
                 quint16 reserved = 0);
    bool sendText(const QHostAddress& host, quint16 port, quint32 baud, const QByteArray& bytes, quint16* ackStatus);

    int timeoutMs = 250;
    int maxAttempts = 3;

private:
    bool ensureSocket();
    bool waitFor(quint32 sequence, quint16 type, int timeoutMs, QByteArray* payload);

    QUdpSocket* m_socket = nullptr;
    quint32 m_sequence = 1;
};
