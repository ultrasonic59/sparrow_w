#include "RespondFrame.h"


RespondFrame::RespondFrame(void) :
				Size(0),
				MaxSize(0),
				Frame(NULL)
{
}

RespondFrame::RespondFrame(quint64 InitMaxSize) :
				Size(0),
				MaxSize(InitMaxSize),
				Frame(NULL)
{
	Frame = new RespondAuto[MaxSize];
	memset(Frame, 0, sizeof(RespondAuto)*MaxSize);
}


RespondFrame::~RespondFrame(void)
{
	if(Frame != NULL)
		delete[] Frame;

	Frame = NULL;
}

quint64 RespondFrame::GetSize() const
{
	return Size;
}

bool RespondFrame::SetSize(const quint64 NewSize)
{
	if(NewSize <= MaxSize)
	{
		Size = NewSize;
	}
	return false;
}


bool RespondFrame::SetNewMaxSize(const quint64 NewMaxSize, const bool save_old)			
{
	RespondAuto *NewFrame = new RespondAuto[NewMaxSize];
	if(NewFrame == NULL)
		return false;

	if(save_old)									
	{
		if(Size < NewMaxSize)
		{
			if(memcpy(NewFrame, Frame, sizeof(RespondAuto) * Size ) == NULL)
				return false;
		}
		else
		{
			if(memcpy(NewFrame, Frame, sizeof(RespondAuto) * NewMaxSize ) == NULL)
				return false;

			Size = NewMaxSize;
		}
	}

	if(Frame != NULL)
		delete[] Frame;
	MaxSize = NewMaxSize;
	Frame = NewFrame;
	return true;
}

quint64 RespondFrame::GetMaxSize() const
{
	return MaxSize;
}

bool RespondFrame::CopyFromArray(const RespondAuto *Src_array, const quint64 resp_count, bool scaling)
{

	if(scaling && !(resp_count < MaxSize))
	{

/*
		Size = MaxSize;

		quint64 counter = 0;
		float sub_counter = 0;
		float interval = (float)resp_count / Size;

		quint32 coll_count = 0;

		quint8 temparr[AD_NUM_TACTS];
		memset(temparr, 0, sizeof(quint8) * AD_NUM_TACTS);

		quint16 type_def = 0;


		for(quint64 i = 0; i < resp_count; i++)
		{
			if((quint64)sub_counter > counter)
			{
				Frame[counter].type_def = type_def;

				for(quint8 j = 0; j < AD_NUM_TACTS; j++)
				{
					Frame[counter].ampl[j] = temparr[j];

				}

				memset(temparr, 0, sizeof(quint8) * AD_NUM_TACTS);
				type_def = 0;
				coll_count = 0;

				counter++;
			}

			type_def = GetPriorityType(type_def, Src_array[resp_count].type_def);

			for(quint8 j = 0; j < AD_NUM_TACTS; j++)
			{
				temparr[j] = (temparr[j] > Src_array[resp_count].ampl[j]) ? temparr[j] : Src_array[resp_count].ampl[j] ;
			}

			coll_count ++;
			counter += interval;
		}
*/
	}
	else
	{
		Size = (MaxSize < resp_count) ? MaxSize : resp_count;
		memcpy(Frame, Src_array, sizeof(RespondAuto) * Size );
	}

	return true;
}

quint16 RespondFrame::GetPriorityType(const quint16 type1, const quint16 type2) const
{
	if(type1 < type2)
		return type2;

	return type1;
}



bool RespondFrame::AddRespondAuto(RespondAuto &resp)
{
	if(Size < MaxSize)
	{
		Frame[Size] = resp;
		Size++;
		return true;
	}
	return false;
}


void RespondFrame::ShiftFrameArray(const qint32 positions)
{
	for(quint64 i = 0; i < Size - positions; i++)
	{
		Frame[i] = Frame[i + positions];
	}
	Size -= positions;
}


void RespondFrame::clear(void)
{
	if(Frame != NULL)
		delete[] Frame;

	Frame = NULL;
	Size = 0;
	MaxSize = 0;
}

const RespondAuto *RespondFrame::GetFrameArray(void) const
{
	return Frame;
}











