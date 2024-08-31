#pragma once
#include "../Definitions.h"

class IHandleEntity;

class CBaseHandle
{
public:
	CBaseHandle();
	CBaseHandle(const CBaseHandle& other);
	CBaseHandle(unsigned long value);
	CBaseHandle(int iEntry, int iSerialNumber);
	void Init(int iEntry, int iSerialNumber);
	void Term();
	bool IsValid() const;
	int GetEntryIndex() const;
	int GetSerialNumber() const;
	int ToInt() const;
	bool operator !=(const CBaseHandle& other) const;
	bool operator ==(const CBaseHandle& other) const;
	bool operator ==(const IHandleEntity* pEnt) const;
	bool operator !=(const IHandleEntity* pEnt) const;
	bool operator <(const CBaseHandle& other) const;
	bool operator <(const IHandleEntity* pEnt) const;
	const CBaseHandle& operator=(const IHandleEntity* pEntity);
	const CBaseHandle& Set(const IHandleEntity* pEntity);
	IHandleEntity* Get() const;

protected:
	unsigned long m_Index;
};

#include "ihandleentity.h"

inline CBaseHandle::CBaseHandle()
{
	m_Index = INVALID_EHANDLE_INDEX;
}

inline CBaseHandle::CBaseHandle(const CBaseHandle& other)
{
	m_Index = other.m_Index;
}

inline CBaseHandle::CBaseHandle(unsigned long value)
{
	m_Index = value;
}

inline CBaseHandle::CBaseHandle(int iEntry, int iSerialNumber)
{
	Init(iEntry, iSerialNumber);
}

inline void CBaseHandle::Init(int iEntry, int iSerialNumber)
{
	m_Index = iEntry | (iSerialNumber << NUM_ENT_ENTRY_BITS);
}

inline void CBaseHandle::Term()
{
	m_Index = INVALID_EHANDLE_INDEX;
}

inline bool CBaseHandle::IsValid() const
{
	return m_Index != INVALID_EHANDLE_INDEX;
}

inline int CBaseHandle::GetEntryIndex() const
{
	return m_Index & ENT_ENTRY_MASK;
}

inline int CBaseHandle::GetSerialNumber() const
{
	return m_Index >> NUM_ENT_ENTRY_BITS;
}

inline int CBaseHandle::ToInt() const
{
	return (int)m_Index;
}

inline bool CBaseHandle::operator !=(const CBaseHandle& other) const
{
	return m_Index != other.m_Index;
}

inline bool CBaseHandle::operator ==(const CBaseHandle& other) const
{
	return m_Index == other.m_Index;
}

inline bool CBaseHandle::operator ==(const IHandleEntity* pEnt) const
{
	return Get() == pEnt;
}

inline bool CBaseHandle::operator !=(const IHandleEntity* pEnt) const
{
	return Get() != pEnt;
}

inline bool CBaseHandle::operator <(const CBaseHandle& other) const
{
	return m_Index < other.m_Index;
}

inline bool CBaseHandle::operator <(const IHandleEntity* pEntity) const
{
	unsigned long otherIndex = (pEntity) ? pEntity->GetRefEHandle().m_Index : INVALID_EHANDLE_INDEX;
	return m_Index < otherIndex;
}

inline const CBaseHandle& CBaseHandle::operator=(const IHandleEntity* pEntity)
{
	return Set(pEntity);
}

inline const CBaseHandle& CBaseHandle::Set(const IHandleEntity* pEntity)
{
	if (pEntity)
	{
		*this = pEntity->GetRefEHandle();
	}
	else
	{
		m_Index = INVALID_EHANDLE_INDEX;
	}

	return *this;
}

template <class T>
class CHandle : public CBaseHandle
{
public:
	CHandle();
	CHandle(int iEntry, int iSerialNumber);
	CHandle(const CBaseHandle& handle);
	CHandle(T* pVal);

	static CHandle<T> FromIndex(int index);

	T* Get() const;
	void Set(const T* pVal);

	operator T* ();
	operator T* () const;

	bool               operator !() const;
	bool               operator==(T* val) const;
	bool               operator!=(T* val) const;
	const CBaseHandle& operator=(const T* val);

	T* operator->() const;
};

template <class T>
CHandle<T>::CHandle()
{
}

template <class T>
CHandle<T>::CHandle(int iEntry, int iSerialNumber)
{
	Init(iEntry, iSerialNumber);
}

template <class T>
CHandle<T>::CHandle(const CBaseHandle& handle) : CBaseHandle(handle)
{
}

template <class T>
CHandle<T>::CHandle(T* pObj)
{
	Term();
	Set(pObj);
}

template <class T>
CHandle<T> CHandle<T>::FromIndex(int index)
{
	CHandle<T> ret;
	ret.m_Index = index;
	return ret;
}

template <class T>
T* CHandle<T>::Get() const
{
	return reinterpret_cast<T*>(CBaseHandle::Get());
}

template <class T>
CHandle<T>::operator T* ()
{
	return Get();
}

template <class T>
CHandle<T>::operator T* () const
{
	return Get();
}

template <class T>
bool CHandle<T>::operator !() const
{
	return !Get();
}

template <class T>
bool CHandle<T>::operator==(T* val) const
{
	return Get() == val;
}

template <class T>
bool CHandle<T>::operator!=(T* val) const
{
	return Get() != val;
}

template <class T>
void CHandle<T>::Set(const T* pVal)
{
	CBaseHandle::Set(reinterpret_cast<const IHandleEntity*>(pVal));
}

template <class T>
const CBaseHandle& CHandle<T>::operator=(const T* val)
{
	Set(val);
	return *this;
}

template <class T>
T* CHandle<T>::operator ->() const
{
	return Get();
}