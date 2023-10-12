// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля


#include "tset.h"


TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования

TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
    BitField = bf;
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if ((Elem < 0) || (Elem >= MaxPower)) throw "<0 or out of range";
    return BitField.GetBit(Elem);
    
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((Elem < 0) || (Elem >= MaxPower)) throw "<0 or out of range";
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ((Elem < 0) || (Elem >= MaxPower)) throw "<0 or out of range";
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this == &s) return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if ((MaxPower == s.MaxPower)&& (BitField == s.BitField)) return 1;
    return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (*this == s) return 0;
    return 1;
}

TSet TSet::operator+(const TSet &s) // объединение                ?
{
    if (*this == s) return *this;
    bool m = (MaxPower < s.MaxPower);
    int l = (m) * s.MaxPower + MaxPower * (!m);
    TSet tmp(l);
    tmp.BitField = BitField |s.BitField;
    return tmp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if ((Elem < 0) || (Elem >= MaxPower)) throw "<0 or out of range";
    BitField.SetBit(Elem);
    return *this;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if ((Elem < 0) || (Elem >= MaxPower)) throw "<0 or out of range";
    BitField.ClrBit(Elem);
    return *this;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet tmp(BitField & s.BitField);
    return tmp;
}

TSet TSet::operator~(void) // дополнение
{
    BitField = ~BitField;
    return *this;
}

istream& operator>>(istream& istr, TSet& bf) {
    istr >> bf.MaxPower;
    istr >> bf.BitField;

    return istr;
};
ostream& operator<<(ostream& ostr, const TSet& bf) {
    ostr << bf.MaxPower << endl;
    ostr << bf.BitField;
    return ostr;
};

