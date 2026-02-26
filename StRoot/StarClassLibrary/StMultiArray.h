/*!
 * \file StMultiArray.h
 * \brief Multidimensional array template, up to 8 dimensions.
 */
#ifndef STMULTIARRAY_H
#define STMULTIARRAY_H

/*!
 * \class StMultiArrayBase
 * \brief Non-templated implementation base for StMultiArray.
 * \note Supports a maximum of 8 dimensions.
 */
class StMultiArrayBase
{
protected:
StMultiArrayBase(int n1,int n2);
StMultiArrayBase(int n1,int n2,int n3);
StMultiArrayBase(int n1,int n2,int n3,int n4);
StMultiArrayBase(int n1,int n2,int n3,int n4,int n5);
StMultiArrayBase(int n1,int n2,int n3,int n4,int n5,int n6);
StMultiArrayBase(int n1,int n2,int n3,int n4,int n5,int n6,int n7);
StMultiArrayBase(int n1,int n2,int n3,int n4,int n5,int n6,int n7,int n8);
void Init(int *sz,int n);
void Clear();
void AddIdx(int i);
int  GetIdx();
int  GetKdx();
public:
static void Test();
private:
int mNDes;
int mDes[8+1];
int mTally;
int mSft;
};

/*!
 * \class StMultiArray
 * \brief Multidimensional array of type T with up to 8 dimensions.
 * \details Subscript with \c operator[] as many times as there are dimensions.
 *          The final subscript returns the element value via \c operator T(),
 *          while casting to \c T* after all-but-one subscripts yields a row pointer
 *          (e.g., \c arr[i][j] gives element value; \c (T*)arr[i] gives the i-th row).
 *          Attach an existing flat C-array with \c operator=(T*).
 */
template<class T>
class StMultiArray :public StMultiArrayBase {
public:
StMultiArray(int n1,int n2):StMultiArrayBase(n1,n2){mA=0;}
StMultiArray(int n1,int n2,int n3):StMultiArrayBase(n1,n2,n3){mA=0;}
StMultiArray(int n1,int n2,int n3,int n4):StMultiArrayBase(n1,n2,n3,n4){mA=0;}
StMultiArray(int n1,int n2,int n3,int n4,int n5):StMultiArrayBase(n1,n2,n3,n4,n5){mA=0;}
StMultiArray(int n1,int n2,int n3,int n4,int n5,int n6):StMultiArrayBase(n1,n2,n3,n4,n5,n6){mA=0;}
StMultiArray(int n1,int n2,int n3,int n4,int n5,int n6,int n7):StMultiArrayBase(n1,n2,n3,n4,n5,n6,n7){mA=0;}
StMultiArray(int n1,int n2,int n3,int n4,int n5,int n6,int n7,int n8):StMultiArrayBase(n1,n2,n3,n4,n5,n6,n7,n8){mA=0;}
/// \brief Implicit conversion to element value T at the currently accumulated index.
/// \return The element value at the computed flat index.
operator T (){return mA[GetIdx()];}
/// \brief Implicit conversion to a row pointer T* at the currently accumulated sub-index.
/// \return Pointer to the start of the current row.
operator T*(){return mA+GetKdx();}
/// \brief Subscript operator; accumulates dimension index and returns self for chaining.
/// \param i  Index along the current dimension.
/// \return Reference to this object for the next subscript in the chain.
StMultiArray &operator[](int i) {AddIdx(i); return *this;}
/// \brief Attach an existing flat C-array as the backing store.
/// \param array  Pointer to the flat array; ownership is not transferred.
void operator=(T *array){mA=array; Clear();}
/// \brief Assign a scalar value to the element at the currently accumulated index.
/// \param value  Value to store.
void operator=(T  value){mA[GetIdx()]=value;}
private:
T   *mA;
};
#endif //STMULTIARRAY_H

