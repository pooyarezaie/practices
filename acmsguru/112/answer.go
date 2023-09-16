package main

import (
	"fmt"
)

type BigInt struct {
	Digits []int32
}

func NewBigInt(i int) *BigInt {
	answer := BigInt{Digits: make([]int32, 0)}
	for ; i > 0; i = i / 10 {
		answer.Digits = append(answer.Digits, int32(i%10))
	}
	return &answer
}

func CopyBigInt(bi *BigInt) *BigInt {
	answer := BigInt{Digits: make([]int32, len(bi.Digits))}
	for i := 0; i < len(bi.Digits); i += 1 {
		answer.Digits[i] = bi.Digits[i]
	}
	return &answer
}

func (bi *BigInt) relax() *BigInt {
	if len(bi.Digits) == 0 {
		return bi
	}
	for i := 0; i < len(bi.Digits)-1; i += 1 {
		if bi.Digits[i] > 9 {
			bi.Digits[i+1] += int32(bi.Digits[i] / 10)
			bi.Digits[i] = bi.Digits[i] % 10
		} else if bi.Digits[i] < 0 {
			bi.Digits[i+1] += bi.Digits[i] / 10
			bi.Digits[i] = (bi.Digits[i]%10 + 10) % 10
			if bi.Digits[i] > 0 {
				bi.Digits[i+1] -= 1
			}
		}
	}
	for bi.Digits[len(bi.Digits)-1] > 9 {
		carry := bi.Digits[len(bi.Digits)-1] / 10
		bi.Digits[len(bi.Digits)-1] = bi.Digits[len(bi.Digits)-1] % 10
		bi.Digits = append(bi.Digits, carry)
	}
	for bi.Digits[len(bi.Digits)-1] < -9 {
		carry := bi.Digits[len(bi.Digits)-1] / 10
		bi.Digits[len(bi.Digits)-1] = (bi.Digits[len(bi.Digits)-1]%10 + 10) % 10
		if bi.Digits[len(bi.Digits)-1] > 0 {
			carry -= 1
		}
		bi.Digits = append(bi.Digits, carry)
	}
	return bi
}

func Add(first, second *BigInt) *BigInt {
	answer := NewBigInt(0)
	var minNum, maxNum *BigInt
	if len(first.Digits) > len(second.Digits) {
		maxNum = first
		minNum = second
	} else {
		maxNum = second
		minNum = first
	}
	for i := 0; i < len(minNum.Digits); i += 1 {
		answer.Digits = append(answer.Digits, minNum.Digits[i]+maxNum.Digits[i])
	}
	for i := len(minNum.Digits); i < len(maxNum.Digits); i += 1 {
		answer.Digits = append(answer.Digits, maxNum.Digits[i])
	}
	answer.relax()
	return answer
}

func (bi *BigInt) Minus() *BigInt {
	answer := CopyBigInt(bi)
	for i := 0; i < len(answer.Digits); i += 1 {
		answer.Digits[i] = -answer.Digits[i]
	}
	return answer
}

func Multiply(first, second *BigInt) *BigInt {
	answer := NewBigInt(0)
	for i := len(first.Digits) - 1; i >= 0; i -= 1 {
		answer = Add(
			MultiplyByInt(answer, 10),
			MultiplyByInt(second, int8(first.Digits[i])),
		)
	}
	answer.relax()
	return answer
}

func MultiplyByInt(first *BigInt, second int8) *BigInt {
	answer := CopyBigInt(first)
	for i := 0; i < len(first.Digits); i += 1 {
		answer.Digits[i] *= int32(second)
	}
	answer.relax()
	return answer
}

func Power(base *BigInt, power int) *BigInt {
	if power == 0 {
		return NewBigInt(1)
	}
	if power == 1 {
		return CopyBigInt(base)
	}
	s := Power(base, power/2)

	if power%2 == 0 {
		return Multiply(s, s)
	} else {
		return Multiply(Multiply(s, s), base)
	}
}

func (bi *BigInt) Print() {
	printNumber := bi
	if bi.Digits[len(bi.Digits)-1] < 0 {
		fmt.Print("-")
		printNumber = bi.Minus()
	}
	printNumber.relax()
	i := len(printNumber.Digits) - 1
	for i > 0 && printNumber.Digits[i] == 0 {
		i -= 1
	}
	for ; i >= 0; i -= 1 {
		fmt.Print(printNumber.Digits[i])
	}
}

func (bi *BigInt) Println() {
	bi.Print()
	fmt.Println()
}

func main() {
	var a, b int
	_, _ = fmt.Scanf("%d %d\n", &a, &b)
	Add(Power(NewBigInt(a), b), Power(NewBigInt(b), a).Minus()).Println()
}
