#pragma once

template <typename ValueClass, typename CountType = int> class TwoDimBuffer
{
public:
	TwoDimBuffer(void);
	~TwoDimBuffer(void);

	void SetArraySize(const CountType len, const CountType dim);

	ValueClass **val_array;

	const CountType GetDim(void) const;
	const CountType GetLen(void) const;

	CountType array_pos;

	bool RotateCurcal(const CountType positions);

	ValueClass * GetValDest(void) const;
	void ClearDim(const CountType dimNum);

protected:


	ValueClass *val_dest;

	CountType array_dim;
	CountType array_len;

};


template <typename ValueClass, typename CountType> TwoDimBuffer<ValueClass, CountType>::TwoDimBuffer(void) :
						val_array(nullptr),
						val_dest(nullptr),
						array_dim(0),
						array_len(0),
						array_pos(0)
{

}


template <typename ValueClass, typename CountType> TwoDimBuffer<ValueClass, CountType>::~TwoDimBuffer(void)
{
	if(val_dest == nullptr)
		return;
	else
	{
		typedef ValueClass * ClassPtr;

		delete[] (reinterpret_cast<ClassPtr>(val_array));
		delete[] val_dest;

		val_array = nullptr;
		val_dest = nullptr;
		array_dim = 0;
		array_len = 0;
		array_pos = 0;
	}
}

template <typename ValueClass, typename CountType> void TwoDimBuffer<ValueClass, CountType>::SetArraySize(const CountType len, const CountType dim)
{
	typedef ValueClass * ClassPtr;

	if(val_dest != nullptr)
	{
		delete[] (reinterpret_cast<ClassPtr>(val_array));
		delete[] val_dest;
		array_pos = 0;
	}

	array_dim = dim;
	array_len = len;

	CountType i, j;

	val_dest = new ValueClass[array_dim * array_len];
	val_array = new ClassPtr[array_len];

	for(i = 0; i < array_len; i++)
		val_array[i] = val_dest + i * array_dim;

	for(i = 0; i < array_len; i++)
		for(j = 0; j < array_dim; j++)
			val_array[i][j] = 0;
}


template <typename ValueClass, typename CountType> const CountType TwoDimBuffer<ValueClass, CountType>::GetDim() const
{
	return array_dim;
}

template <typename ValueClass, typename CountType> const CountType TwoDimBuffer<ValueClass, CountType>::GetLen() const
{
	return array_len;
}

template <typename ValueClass, typename CountType> ValueClass * TwoDimBuffer<ValueClass, CountType>::GetValDest(void) const
{
	return val_dest;
}


template <typename ValueClass, typename CountType> bool TwoDimBuffer<ValueClass, CountType>::RotateCurcal(const CountType positions)
{
	if(array_len < positions)
	{
		array_pos = positions - array_len;
		return false;
	}

	if((array_pos + positions) <= array_len)
		return true;

	CountType displ = array_pos - (array_len - positions);
	array_pos = array_len - positions;


	for(CountType j = 0; j < array_dim; j++)
	{
		for(CountType i = 0; i < array_pos; i++)
			val_array[i][j] = val_array[i + displ][j];
	}

	return true;
}


template <typename ValueClass, typename CountType> void TwoDimBuffer<ValueClass, CountType>::ClearDim(const CountType dimNum)
{
	for(CountType i = 0; i < array_len; i++)
		val_array[i][dimNum] = 0;
}