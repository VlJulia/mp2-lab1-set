
// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле


#include "tbitfield.h"
#include <ostream>

// Fake variables used as placeholders in tests

static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) {
        //return; //____________________________________________________!!!!!!!!!!!!!!!!!
        throw "len<=0";
    }
    BitLen = len;//во всех

    MemLen = (len / (sizeof(TELEM)) + (len % (sizeof(TELEM)) !=0));//блоки
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) pMem[i] = 0;

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    if (pMem == nullptr) return;
    delete[] pMem;
    BitLen = 0;
    pMem = nullptr;
        
}
// индекс Мем для бита n
int TBitField::GetMemIndex(const int n) const 
{
    if ((n < 0) || (n >= BitLen)) {
        //____________________________________________________!!!!!!!!!!!!!!!!!
        throw "index is out of range or <0";
    }
    return n / (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
{

    if ((n < 0) || (n >= BitLen)) {
//____________________________________________________!!!!!!!!!!!!!!!!!
        throw "index is out of range or <0";
    }
    return (1 << (n - 1) % (sizeof(TELEM) * 8));//size of в байтах,а надо в битах
    
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит +++++++++++++++++++++++++++++++++++++++++
{   
    if ((n < 0)|| (n >= BitLen)) {
       //____________________________________________________!!!!!!!!!!!!!!!!!
        throw "index <0 or out of range"; }

    pMem[GetMemIndex(n)]|=GetMemMask(n);

}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen)) {
       //____________________________________________________!!!!!!!!!!!!!!!!!
        throw "index is out of range or <0"; }

    pMem[GetMemIndex(n)] &= (~GetMemMask(n));

}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen)) { 
      //____________________________________________________!!!!!!!!!!!!!!!!!
        throw "index is out of range or <0"; }
    return (pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        delete[]pMem;
        BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}
    }

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if ((MemLen != bf.MemLen) || (BitLen != bf.BitLen)) return 0;
    for (int i = 0; i < MemLen; i++) if (pMem[i] != bf.pMem[i]) return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if ((MemLen != bf.MemLen) || (BitLen != bf.BitLen)) return 1;
    for (int i = 0; i < BitLen; i++) if (pMem[i] != bf.pMem[i]) return 1;
    return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    if (*this == bf) return *this;
    TBitField tmp(1);

    if (BitLen > bf.BitLen) {
        tmp = *this;
        for (int i = 0; i < MemLen; i++) tmp.pMem[i] = pMem[i];
        for (int i = 0; i < bf.MemLen; i++) tmp.pMem[i] |= bf.pMem[i];

        std::cout << "--" << tmp << "--" << bf << "--" << *this;
        return tmp;
    }
    else
    {
        tmp = bf;
        for (int i = 0; i < bf.MemLen; i++) tmp.pMem[i] = bf.pMem[i];
        for (int i = 0; i < MemLen; i++) tmp.pMem[i] |= pMem[i];
        std::cout << "--" << tmp << "--" << bf << "--" << *this;
        return tmp;
    }
}


TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    
    if (*this == bf) return *this;
    bool m = (BitLen > bf.BitLen);
    int l = m * BitLen + (!m) * bf.BitLen;
    TBitField tmp(l);

    for (int i = 0; i < MemLen; i++) tmp.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++) tmp.pMem[i] &= bf.pMem[i];
    return tmp;


}

TBitField TBitField::operator~(void) // отрицание
{
    
    TBitField tmp(BitLen);

    for (int i = 0; i < BitLen; i++) {
        if (tmp.GetBit(i)) {
            tmp.ClrBit(i);
        }
        else
        {
            tmp.SetBit(i);
        }
    }

    return tmp;

}

// ввод/вывод

istream& operator>>(istream &istr, TBitField& bf) 
{
    string tmp;
    istr >> tmp;
    for (int i = 0; i < bf.BitLen; i++) {
        switch (tmp[i])
        {
        case '0': {bf.ClrBit(i);  break; }
        case '1': {bf.SetBit(i); break; }

        default:
            throw "unexcepted value ";
        }
    }
    return istr;
};

ostream& operator<<(ostream &ostr, const TBitField& bf) {

    for (int i = 0; i < bf.GetLength(); i++) {
        for (int i = 0; i < bf.BitLen; i++) {
            switch (bf.GetBit(i))
            {
            case 0: {ostr << '0'; break; }
            case 1: {ostr << '1'; break; }
            default: throw "unexcepted value in array";
            }
        }

    } 
    return ostr;
};
