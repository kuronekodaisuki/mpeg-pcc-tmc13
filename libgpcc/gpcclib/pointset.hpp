/**
* @file pointset.hpp
* @brief PointSet3クラスが定義されたヘッダ
* @author KDDI CORPORATION
* @details PointSet3クラスが定義されたヘッダ。詳細な内容は各クラス、構造体、などを参照の事。
*/
#pragma once

#include <cstdint>
#include "gpcclib/def.hpp"
#include "gpcclib/common.hpp"

/**
* @brief gpcclib名前空間
* @details GPCC SDKを使う場合に使用する名前空間
*/
namespace gpcclib {

// The type used for internally representing attribute data
typedef uint16_t attr_t;

/**
* @brief 点群情報を保持するクラス
* @details 点群情報を保持するクラス
*/
class GPCCLIB_INTERFACE_SPEC PointSet3 {
public:
  typedef point_t PointType;
  typedef Vec3<attr_t> AttrVec3;
  typedef Box3<int32_t> BoundingBoxType;

  /**
  * @brief インスタンス生成
  * @details PointSet3クラスのインスタンスを生成する
  * @return PointSet3* 生成したPointSet3インスタンスのポインタ
  */
  static PointSet3* create();

  /**
  * @brief インスタンス解放
  * @details PointSet3クラスのインスタンスを解放する
  */
  virtual void release() = 0;

  /**
  * @brief 点の数を取得
  * @details 保持している点の数を取得する
  * @return size_t 点の数
  */
  virtual size_t getPointCount() const = 0;

  /**
  * @brief 指定された点の座標情報を取得
  * @details indexで指定されて点の座標情報を取得する
  * @param[in] index: 頂点を取得したい点のインデックス
  * @return PointType 座標情報
  */
  virtual PointType getPosition(size_t index) const = 0;

  /**
  * @brief 指定された点の座標情報を取得
  * @details indexで指定されて点の座標情報を取得する
  * @param[in] index: 座標情報を取得したい点のインデックス
  * @return PointType 座標情報の実体
  */
  virtual PointType& getPosition(size_t index) = 0;

  /**
  * @brief 指定された点の座標情報を設定
  * @details indexで指定されて点の座標情報を設定する
  * @param[in] index: 座標情報を取得したい点のインデックス
  * @param[in] position: 設定したい座標情報
  */
  virtual void setPosition(size_t index, const PointType& position) = 0;

  /**
  * @brief 指定された点の色情報を取得
  * @details indexで指定されて点の色情報を取得する
  * @param[in] index: 色情報を取得したい点のインデックス
  * @return AttrVec3 色情報の実体
  */
  virtual AttrVec3 getColor(size_t index) const = 0;

  /**
  * @brief 指定された点の色情報を取得
  * @details indexで指定されて点の色情報を取得する
  * @param[in] index: 色情報を取得したい点のインデックス
  * @return AttrVec3 色情報の実体
  */
  virtual AttrVec3& getColor(size_t index) = 0;

  /**
  * @brief 指定された点の色情報を設定
  * @details indexで指定されて点の色情報を設定する
  * @param[in] index: 色情報を設定したい点のインデックス
  * @param[in] AttrVec3 色情報の実体
  */
  virtual void setColor(size_t index, const AttrVec3& color) = 0;

  /**
  * @brief 色情報の設定状況を取得
  * @details 色情報の設定状況を取得する
  * @return TRUE:色情報が設定有 FALSE:色情報が未設定
  */
  virtual bool hasColors() const = 0;

  /**
  * @brief 色情報の設定状況を更新
  * @details 色情報の設定状況を有りに更新する
  */
  virtual void addColors() = 0;

  /**
  * @brief 色情報の設定状況を更新
  * @details 色情報の設定状況を除去し無しに更新する
  */
  virtual void removeColors() = 0;

  /**
  * @brief 指定された点の反射情報を取得
  * @details indexで指定されて点の反射情報を取得する
  * @param[in] index: 反射情報を取得したい点のインデックス
  * @return attr_t 反射情報
  */
  virtual attr_t getReflectance(size_t index) const = 0;

  /**
  * @brief 指定された点の反射情報を取得
  * @details indexで指定されて点の反射情報を取得する
  * @param[in] index: 反射情報を取得したい点のインデックス
  * @return attr_t& 反射情報の実体
  */
  virtual attr_t& getReflectance(size_t index) = 0;

  /**
  * @brief 指定された点の反射情報を設定
  * @details indexで指定されて点の反射情報を設定する
  * @param[in] index: 反射情報を取得したい点のインデックス
  * @param[in] reflectance: 反射情報の実体
  */
  virtual void setReflectance(size_t index, const attr_t& reflectance) = 0;

  /**
  * @brief 反射情報の設定状況を取得
  * @details 反射情報の設定状況を取得する
  * @return TRUE:反射情報の設定有 FALSE:反射情報が未設定
  */
  virtual bool hasReflectances() const = 0;

  /**
  * @brief 反射情報の設定状況を更新
  * @details 反射情報の設定状況を有りに更新する
  */
  virtual void addReflectances() = 0;

  /**
  * @brief 反射情報の設定状況を更新
  * @details 反射情報の設定状況を除去し無しに更新する
  */
  virtual void removeReflectances() = 0;

  /**
  * @brief 指定された点のフレーム番号を取得
  * @details indexで指定されて点のフレーム番号を取得する
  * @param[in] index: フレーム番号を取得したい点のインデックス
  * @return uint8_t フレーム番号
  */
  virtual uint8_t getFrameIndex(size_t index) const = 0;

  /**
  * @brief 指定された点のフレーム番号を取得
  * @details indexで指定されて点のフレーム番号を取得する
  * @param[in] index: フレーム番号を取得したい点のインデックス
  * @return uint8_t& フレーム番号の実体
  */
  virtual uint8_t& getFrameIndex(size_t index) = 0;

  /**
  * @brief 指定された点のフレーム番号を取得
  * @details indexで指定されて点のフレーム番号を取得する
  * @param[in] index: フレーム番号を取得したい点のインデックス
  * @param[in] frameIndex: フレーム番号
  */
  virtual void setFrameIndex(size_t index, uint8_t frameIndex) = 0;

  /**
  * @brief フレーム番号の設定状況を取得
  * @details フレーム番号の設定状況を取得する
  * @return TRUE:フレーム番号の設定有 FALSE:フレーム番号が未設定
  */
  virtual bool hasFrameIndex() const = 0;

  /**
  * @brief フレーム番号の設定状況を更新
  * @details フレーム番号の設定状況を有りに更新する
  */
  virtual void addFrameIndex() = 0;

  /**
  * @brief 反射情報の設定状況を更新
  * @details 反射情報の設定状況を除去し無しに更新する
  */
  virtual void removeFrameIndex() = 0;

  /**
  * @brief 色情報と反射情報の設定状況を更新
  * @details 色情報と反射情報の設定状況を更新する
  * @param[in] withColors: 色情報の設定状況
  * @param[in] withReflectances: 反射情報の設定状況
  */
  virtual void addRemoveAttributes(bool withColors, bool withReflectances) = 0;

  /**
   * @brief 他のオブジェクトと入れ替える
   * @details 他の点情報オブジェクトと入れ替える
   * @param[in] other: 入れ替える点情報の実体
   */
  virtual void swap(PointSet3& other) = 0;

  /**
  * @brief 要素数を変更する
  * @details 現在保持している要素数を変更する
  * @param[in] size: 設定する要素数
  */
  virtual void resize(size_t size) = 0;

  /**
  * @brief アロケーション容量を変更する
  * @details アロケーション容量を変更する
  * @param[in] size: 設定するアロケーション数
  */
  virtual void reserve(size_t size) = 0;

  /**
  * @brief 全ての要素を削除する
  * @details 全ての要素を削除し、要素を指す全ての参照、ポインタ、イテレータが無効になる
  */
  virtual void clear() = 0;

  /**
  * @brief 末尾に点群情報を追加する
  * @details 現在の点群情報の末尾に点群情報を追加する
  * @param[in] src: 追加する点群情報の実体
  */
  virtual void append(const PointSet3& src) = 0;

  /**
  * @brief 指定された点の入替
  * @details 指定されたindexの点同士を入れ替える
  * @param[in] index1: 入れ替えたい点のindex
  * @param[in] index2: 入れ替えたい点のindex
  */
  virtual void swapPoints(const size_t index1, const size_t index2) = 0;

  /**
  * @brief 点群すべてが入るボックス情報を取得
  * @details 保持している点群すべてが入るボックス情報を取得する
  * @param[in] index1: 入れ替えたい点のindex
  * @param[in] index2: 入れ替えたい点のindex
  * @return BoundingBoxType 各軸のmaxとminを持ったボックス情報
  */
  virtual BoundingBoxType computeBoundingBox() const = 0;

protected:
  /**
   * @brief PointSet3クラスのコンストラクタ
   * @details PointSet3クラスのデフォルトコンストラクタ指定
   */
  PointSet3() = default;
  /**
   * @brief PointSet3クラスのデストラクタ
   * @details PointSet3クラスのデフォルトデストラクタ指定
   */
  virtual ~PointSet3() = default;
  /**
   * @brief PointSet3クラスの移動コンストラクタ
   * @details 別のインスタンスから移動してあらたなインスタンスを生成します。また、例外をスローしません。
   */
  explicit PointSet3(PointSet3&&) noexcept = default;

private:
  /**
   * @brief PointSet3クラスのコピーコンストラクタ
   * @details PointSet3クラスのコピーコンストラクタを禁止します
   */
  PointSet3(const PointSet3&) = delete;

  /**
   * @brief PointSet3クラスの代入演算子
   * @details PointSet3クラスの代入演算子によるコピーを禁止します
   */
  PointSet3& operator=(const PointSet3&) = delete;

  /**
   * @brief PointSet3クラスの代入演算子
   * @details PointSet3クラスの代入演算子による移動を禁止します
   */
  PointSet3& operator=(PointSet3&&) noexcept = delete;
};

} // namespace gpcclib
