/**
* @file common.hpp
* @brief GPCCコーデックで共通使用されるクラスが記載されたヘッダ
* @author KDDI CORPORATION
* @details GPCCコーデックで使用する共通クラスに関するヘッダ。詳細な内容は各クラス参照
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include "gpcclib/def.hpp"

/**
* @brief gpcclib名前空間
* @details GPCC SDKを使う場合に使用する名前空間
*/
namespace gpcclib {

//============================================================================

/**
* @brief gpcclibのバージョン情報
* @details gpcclibのバージョン情報（ major/minor/patch)を格納 
*/
class GPCCLIB_INTERFACE_SPEC LibraryInfo {
public:
  /**
   * @brief LibrayInfoクラスのmajor,moinor,patchを保持する構造体
   */
  struct Version {
    //! メジャー番号
    unsigned int major;
    //! マイナー番号
    unsigned int minor;
    //! パッチ番号
    unsigned int patch;
  };
  /**
   * @brief LibrayInfoクラスで保持するバージョンを返す
   * @param[in] void
   * @param[out] Version　構造体Version(major,minor,patch) 
   */
  static const Version& getVersion();
};

//============================================================================
/**
* @brief Vec3テンプレートクラス
* @details xyz座標位置を示すVec3テンプレートクラス
* @tparam T 座標の型式 　 
*/
template<typename T>
class GPCCLIB_INTERFACE_SPEC Vec3 {
public:
  // //! パラメータTの型をValueTypeに指定
  typedef T ValueType;
  /**
  * @brief Vec3クラスのコンストラクタ
  * @details Vec3クラスのデフォルトコンストラクタ指定
  */
  Vec3() = default;
  /**
  * @brief Vec3クラスのデストラクタ
  * @details Vec3クラスのデフォルトデストラクタ指定
  */
  ~Vec3() = default;
  /**
  * @brief Vec3クラスのコピーコンストラクタ
  * @details 引数Vec3のコピーコンストラクタ指定
  * @param [in] Vec3 コピー元Vec3クラス(xyz座標)
  */
  Vec3(const Vec3&) = default;
  /**
  * @brief Vec3クラスのムーブコンストラクタ
  * @details 引数がVec3のムーブコンストラクタ指定で明示的に呼ぶ必要あり。例外をスローしない
  * @param [in] Vec3 ムーブ元Vec3クラス(xyz座標)
  */
  explicit Vec3(Vec3&&) noexcept = default;

  //explicit Vec3(const T a);
  /**
  * @brief Vec3クラスのコンストラクタ
  * @details Vec3クラスの引数xyzを指定したコンストラクタ
  * @param [in] x: パラメータT型X座標
  * @param [in] y: パラメータT型Y座標
  * @param [in] z: パラメータT型Z座標
  */
  Vec3(const T x, const T y, const T z);
  /**
  * @brief Vec3クラスの代入演算子
  * @details 引数がVec3時の代入演算子
  * @param [in] rhs: Vec3クラス（代入元）
  * @return Vec3 Vec3クラス（代入先）
  */
  Vec3& operator=(const Vec3& rhs);
  /**
  * @brief Vec3クラスの加算代入演算子
  * @details 引数が1つの時の加算代入演算子　引数を同一加算
  * @param [in] rhs: 加算値
  * @return Vec3 Vec3クラス（代入先）
  */
  Vec3& operator+=(const T rhs);
  /**
  * @brief Vec3クラスの加算代入演算子
  * @details 引数がVec3クラスの加算代入演算子　Vec3を加算
  * @param [in] rhs: Vec3クラス（加算値）
  * @return Vec3 Vec3クラス（加算先）
  */
  Vec3& operator+=(const Vec3& rhs);
  /**
  * @brief Vec3クラスの減算代入演算子
  * @details 引数が1つの時の減算代入演算子　引数を同一減算
  * @param [in] rhs: 減算値
  * @return Vec3 Vec3クラス（減算先）
  */
  Vec3& operator-=(const T rhs);
  /**
  * @brief Vec3クラスの減算代入演算子
  * @details 引数がVec3クラスの減算代入演算子　Vec3を減算
  * @param [in] rhs: Vec3クラス（減算値）
  * @return Vec3 Vec3クラス（減算先）
  */
  Vec3& operator-=(const Vec3& rhs);

  // friend Vec3 operator+(const Vec3& lhs, T rhs);
  // friend Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
  // friend Vec3 operator-(const Vec3& lhs, T rhs);
  // friend Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
  
  /**
  * @brief 先頭データ取得
  * @details Vec3クラスの先頭データ取得
  * @param[in] void
  * @return T* パラメータT型の先頭データのポインタ
  */
  T* begin();
  
  /**
  * @brief 先頭データ取得(変更不可)
  * @details Vec3クラスの先頭データ取得（変更不可）
  * @param[in] void
  * @return T* パラメータT型の先頭データ（変更不可）のポインタ
  */
  const T* begin() const;
  
  /**
  * @brief 次データ取得
  * @details Vec3クラスの次のデータ取得
  * @param[in] void
  * @return T* パラメータT型の次のデータのポインタ
  */
  T* end();

  /**
  * @brief 次データ取得(変更不可)
  * @details Vec3クラスの次のデータ取得（変更不可）
  * @param[in] void
  * @return T* パラメータT型の次のデータ（変更不可）のポインタ
  */
  const T* end() const;
  
  /**
  * @brief 指定データ取得
  * @details Vec3クラスの i番目のデータ取得
  * @param[in] i: データ位置指定
  * @return T& パラメータT型の指定されたデータ
  */
  T& operator[](size_t i);

  /**
  * @brief 指定データ取得(変更不可)
  * @details Vec3クラスの i番目のデータ取得(変更不可)
  * @param[in] i: データ位置指定
  * @return T& パラメータT型の指定されたデータ(変更不可)
  */
  const T& operator[](size_t i) const;
  
  /**
  * @brief Vec3のデータ個数取得
  * @details Vec3クラスのデータ個数取得(3)
  * @param[in] void
  * @return  3
  */
  size_t getElementCount() const;

  /**
  * @brief xデータ取得
  * @details xデータ(data[0])取得
  * @param[in] void
  * @return パラメータT型x座標
  */
  T& x();

  /**
  * @brief yデータ取得
  * @details yデータ(data[1])取得
  * @param[in] void
  * @return パラメータT型y座標
  */
  T& y();

  /**
  * @brief zデータ取得
  * @details zデータ(data[2])取得
  * @param[in] void
  * @return パラメータT型z座標
  */
  T& z();

  /**
  * @brief xデータ取得(変更不可)
  * @details xデータ(data[0])取得(変更不可)
  * @param[in] void
  * @return パラメータT型x座標(変更不可)
  */
  const T& x() const;

  /**
  * @brief yデータ取得(変更不可)
  * @details yデータ(data[1])取得(変更不可)
  * @param[in] void
  * @return パラメータT型y座標(変更不可)
  */
  const T& y() const;

  /**
  * @brief zデータ取得(変更不可)
  * @details zデータ(data[2])取得(変更不可)
  * @param[in] void
  * @return パラメータT型z座標(変更不可)
  */
  const T& z() const;

  /**
  * @brief xデータ取得
  * @details xデータ(data[0])取得
  * @param[in] void
  * @return パラメータT型x座標
  */
  T& s();
  
  /**
  * @brief yデータ取得
  * @details yデータ(data[1])取得
  * @param[in] void
  * @return パラメータT型y座標
  */
  T& t();

  /**
  * @brief zデータ取得
  * @details zデータ(data[2])取得
  * @param[in] void
  * @return パラメータT型z座標
  */
  T& v();

  /**
  * @brief xデータ取得(変更不可)
  * @details xデータ(data[0])取得(変更不可)
  * @param[in] void
  * @return パラメータT型x座標(変更不可)
  */
  const T& s() const;

  /**
  * @brief yデータ取得(変更不可)
  * @details yデータ(data[1])取得(変更不可)
  * @param[in] void
  * @return パラメータT型y座標(変更不可)
  */
  const T& t() const;

  /**
  * @brief zデータ取得(変更不可)
  * @details zデータ(data[2])取得(変更不可)
  * @param[in] void
  * @return パラメータT型z座標(変更不可)
  */
  const T& v() const;

  // The minimum element
  /**
  * @brief 最小データ取得
  * @details Vec3の内、最小データを取得
  * @param[in] void
  * @return Vec3の最小値（パラメータT型）
  */
  T min() const;

  // The maximum element
  /**
  * @brief 最大データ取得
  * @details Vec3の内、最大データを取得
  * @param[in] void
  * @return Vec3の最大値（パラメータT型）
  */
  T max() const;

protected:
  T data[3];
};

//---------------------------------------------------------------------------

// The type used for internally representing positions
/**
* @brief point_t型定義
* @details point_t型をVec3<int32_t>として定義
*/
typedef Vec3<int32_t> point_t;

//---------------------------------------------------------------------------
/**
* @brief Box3テンプレートクラス
* @details 最小の座標と最大の座標を示すBox3テンプレートクラス
* @tparam T 座標の型式 　 
*/
template<typename T>
class GPCCLIB_INTERFACE_SPEC Box3 {
public:
  /**
  * @brief PointType型定義
  * @details PointType型をVec3<T>として定義
  */
  typedef Vec3<T> PointType;
  
  /**
  * @brief Box3クラスのコンストラクタ
  * @details Vec3クラスのデフォルトコンストラクタ指定
  */
  Box3() = default;

  /**
  * @brief Box3クラスのコピーコンストラクタ
  * @details 引数Box3のコピーコンストラクタ指定
  * @param [in] rhs: コピー元Box3クラス
  */
  Box3(const Box3& rhs) = default;
  // Box3(T min, T max);

  /**
  * @brief Box3クラスのコンストラクタ
  * @details 引数がPointType型min,max(変更不可)のコンストラクタ指定
  * @param [in] min: PointType型のmin座標
  * @param [in] max: PointType型のmax座標
  */
  Box3(const PointType& min, const PointType& max);

  /**
  * @brief Box3クラスの代入演算子
  * @details 引数がBox3時の代入演算子
  * @param [in] rhs: Box3クラス（代入元）
  * @return Box3 Box33クラス（代入先）
  */
  Box3& operator=(const Box3& rhs);
  
  /**
  * @brief 指定pointがBox3に含まれるかを返すメソッド
  * @details 引数の指定pointがBox3の最小値、最大値に含まれるかを返す
  * @param [in] point: 指定ポイント（PointType型）
  * @return Box3の最小値、最大値に含まれる場合：True、含まれない場合：False
  */
  bool contains(const PointType& point) const;

  /**
  * @brief 指定pointでBox3を更新するメソッド
  * @details 引数の指定pointと現Box3において最小値、最大値を更新する
  * @param [in] point: 指定ポイント（PointType型）
  * @return 最小値、最大値更新後のBox3
  */
  Box3 merge(const Box3& box);

  /**
  * @brief 指定boxと交差しているか判定するメソッド
  * @details 引数の指定boxと現Box3において交差しているか判定する（boxとBox3に共有座標が存在するか）
  * @param [in] point: 指定ポイント（PointType型）
  * @return 最小値、最大値更新後のBox3
  */
  bool intersects(const Box3& box) const;

  /**
  * @brief 指定pointで最大値、最小値を更新するメソッド
  * @details 指定pointと現Box3の最大値、最小値を更新するメソッド
  * @param [in] point: 指定ポイント（PointType型）
  * @return 最小値、最大値更新後のBox3
  */
  void insert(const PointType& point);

  /**
  * @brief 最小座標を取得するメソッド
  * @details xyz各座標の最小値を取得するメソッド
  * @param [in] void
  * @return min: xyzの最小値(const)
  */
  const PointType& getMin() const;

  /**
  * @brief 最小座標を取得するメソッド
  * @details xyz各座標の最小値を取得するメソッド
  * @param [in] void
  * @return min: xyzの最小値
  */
  PointType& getMin();
  
  /**
  * @brief 最大座標を取得するメソッド
  * @details xyz各座標の最大値を取得するメソッド
  * @param [in] void
  * @return min: xyzの最大値(const)
  */
  const PointType& getMax() const;

  /**
  * @brief 最大座標を取得するメソッド
  * @details xyz各座標の最大値を取得するメソッド
  * @param [in] void
  * @return min: xyzの最大値
  */
  PointType& getMax();

protected:
  PointType min;
  PointType max;
};

//---------------------------------------------------------------------------
/**
* @brief 比率を示す構造体
* @details 比率をint型numerator(分子)／denominator(分母)形式で表す構造体。float型をint型比率で表す
*/
struct GPCCLIB_INTERFACE_SPEC Rational {
  //! 分子
  int numerator;
  //! 分母
  int denominator;
  //! コンストラクタ　分子:0、分母:1
  Rational();
  //! コンストラクタ　分子:numerator, 分母:1
  Rational(int numerator);
  //! コンストラクタ　分子:numerator, 分母:denominator
  Rational(int numerator, int denominator);
  //! コンストラクタ　float型val値を分子:numerator, 分母:denominatorで格納する
  Rational(float val);
  //! コンストラクタ　double型val値を分子:numerator, 分母:denominatorで格納する
  Rational(double val);
  //! 比率をdouble(numerator) / double(denominator)で返すオペレータ
  operator double() const;
  //! 比率をfloat(numerator) / float(denominator)で返すオペレータ
  operator float() const;
};

/**
* @brief ベクタアダプタテンプレートクラス
* @details ベクタ列に関する様々なテンプレートクラス
* @tparam T　ベクタの型 　 
*/
template<typename T>
class GPCCLIB_INTERFACE_SPEC VectorAdapter {
public:
  
  /**
  * @brief パラメータTの再定義
  * @details パラメータTをValueTypeとしてベクタの型に再定義
  */
  typedef T ValueType;
  
  /**
  * @brief ベクタの指定データ取得(const)
  * @details ベクタの i番目のベクタデータ取得(const)
  * @param[in] i: データ位置指定
  * @return ValueType 指定されたベクタデータ(const)
  */
  virtual const ValueType& operator[](size_t index) const = 0;
  
  /**
  * @brief ベクタの指定データ取得
  * @details ベクタの i番目のベクタデータ取得
  * @param[in] i: データ位置指定
  * @return ValueType 指定されたベクタデータ
  */
  virtual ValueType& operator[](size_t index) = 0;
  
  /**
  * @brief ベクタの空判定
  * @details ベクタの空判定
  * @return TRUE:空 FALSE:空でない
  */
  virtual bool isEmpty() const = 0;
  
  /**
  * @brief ベクタの空判定
  * @details ベクタの空判定
  * @return TRUE:空 FALSE:空でない
  */
  virtual size_t getSize() const = 0;

  /**
  * @brief ベクタの要素数を取得
  * @details ベクタの全要素数を取得
  * @return ベクタの全要素数
  */
  virtual size_t getCapacity() const = 0;

  /**
  * @brief ベクタの先頭データ取得
  * @details 先頭ベクタのポインタ取得
  * @return 先頭ベクタのポインタ
  */
  virtual const ValueType* getData() const = 0;
  
  /**
  * @brief ベクタの先頭データ取得
  * @details 先頭ベクタのポインタ取得
  * @return 先頭ベクタのポインタ
  */
  virtual ValueType* getData() = 0;

  /**
  * @brief ベクタのクリア
  * @details 全ての要素を削除し、要素を指す全ての参照、ポインタ、イテレータが無効になる
  */
  virtual void clear() = 0;

  /**
  * @brief ベクタ要素数を変更する
  * @details 現在保持しているベクタの要素数を変更する
  * @param[in] size: 設定する要素数
  */
  virtual void resize(size_t size) = 0;

  /**
  * @brief ベクタ容量を変更する
  * @details ベクタが保持する容量を変更する
  * @param[in] capacity: 設定するベクタ数
  */
  virtual void reserve(size_t capacity) = 0;

  /**
  * @brief ベクタの追加
  * @details ベクタを追加(emplace_back)
  * @param[in] val: 追加するベクタ
  */
  virtual void pushBack(const ValueType& val) = 0;

protected:
  VectorAdapter() = default;
  virtual ~VectorAdapter() = default;
  explicit VectorAdapter(VectorAdapter&&) noexcept = default;

private:
  VectorAdapter(const VectorAdapter&) = delete;
  VectorAdapter& operator=(const VectorAdapter&) = delete;
  VectorAdapter& operator=(const VectorAdapter&&) = delete;
};

/**
* @brief Int32VectorAdapter定義：int32型
* @details int32型のベクタ列定義
*/
typedef VectorAdapter<int32_t> Int32VectorAdapter;

/**
* @brief UInt32VectorAdapter定義：uint32型
* @details uint32型のベクタ列定義
*/
typedef VectorAdapter<uint32_t> UInt32VectorAdapter;

/**
* @brief Int8VectorAdapter定義：int8型
* @details int8型のベクタ列定義
*/
typedef VectorAdapter<int8_t> Int8VectorAdapter;

/**
* @brief UInt8VectorAdapter定義：uint8型
* @details uint8型のベクタ列定義
*/
typedef VectorAdapter<uint8_t> UInt8VectorAdapter;

/**
* @brief DoubleVectorAdapter定義：double型
* @details double型のベクタ列定義
*/
typedef VectorAdapter<double> DoubleVectorAdapter;

/**
* @brief オブジェクト型ベクタのアダプタテンプレートクラス
* @details オブジェクト型ベクタに関する様々なテンプレートクラス
* @tparam T　オブジェクトの型 　 
*/
template<typename T>
class GPCCLIB_INTERFACE_SPEC ObjectVectorAdapter {
public:
  typedef T ObjectType;
  
  /**
  * @brief オブジェクト型ベクタの指定データ取得(const)
  * @details ベクタの i番目のベクタデータ取得(const)
  * @param[in] index: データ位置指定
  * @return ObjectType 指定されたベクタデータ(const)
  */
  virtual const ObjectType& operator[](size_t index) const = 0;

  /**
  * @brief オブジェクト型ベクタの指定データ取得
  * @details ベクタの i番目のベクタデータ取得
  * @param[in] index: データ位置指定
  * @return ObjectType 指定されたベクタデータ
  */
  virtual ObjectType& operator[](size_t index) = 0;
  
  /**
  * @brief オブジェクト型ベクタが空であるか
  * @details オブジェクト型ベクタが空であるか
  * @return TRUE:空 FALSE:空でない
  */
  virtual bool isEmpty() const = 0;

  /**
  * @brief オブジェクト型ベクタの個数取得
  * @details オブジェクト型ベクタのオブジェクト数取得
  * @return ベクタ内のオブジェクト数
  */
  virtual size_t getSize() const = 0;
  
  /**
  * @brief オブジェクト型ベクタの容量取得
  * @details オブジェクト型ベクタの現容量取得
  * @return ベクタに格納できるオブジェクト数
  */
  virtual size_t getCapacity() const = 0;

  /**
  * @brief オブジェクト型ベクタの指定オブジェクト取得(const)
  * @details オブジェクト型ベクタの指定オブジェクト取得(const)
  * @param[in] index: データ位置指定
  * @return ベクタ内の指定オブジェクト(const)
  */
  virtual const ObjectType& at(size_t index) const = 0;

  /**
  * @brief オブジェクト型ベクタの指定オブジェクト取得
  * @details オブジェクト型ベクタの指定オブジェクト取得
  * @param[in] index: データ位置指定
  * @return ベクタ内の指定オブジェクト
  */
  virtual ObjectType& at(size_t index) = 0;
  
  /**
  * @brief オブジェクト型ベクタのクリア
  * @details 全ての要素を削除し、要素を指す全ての参照、ポインタ、イテレータが無効になる
  */
  virtual void clear() = 0;
  
  /**
  * @brief オブジェクト型ベクタ要素数を変更する
  * @details 現在保持しているベクタの要素数を変更する
  * @param[in] size: 設定する要素数
  */
  virtual void resize(size_t size) = 0;

  /**
  * @brief オブジェクト型ベクタ容量を変更する
  * @details ベクタが保持する容量を変更する
  * @param[in] capacity: 設定するベクタ数
  */
  virtual void reserve(size_t capacity) = 0;
  
  /**
  * @brief オブジェクトの追加
  * @details オブジェクトを追加(emplace_back)
  * @param[in] val: 追加するオブジェクト
  */  
  virtual void pushBack(const ObjectType& obj) = 0;

protected:
  ObjectVectorAdapter() = default;
  virtual ~ObjectVectorAdapter() = default;
  explicit ObjectVectorAdapter(ObjectVectorAdapter&&) noexcept = default;

private:
  ObjectVectorAdapter(const ObjectVectorAdapter&) = delete;
  ObjectVectorAdapter& operator=(const ObjectVectorAdapter&) = delete;
  ObjectVectorAdapter& operator=(const ObjectVectorAdapter&&) = delete;
};

} // namespace gpcclib
