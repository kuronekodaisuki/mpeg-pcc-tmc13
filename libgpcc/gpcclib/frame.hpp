/**
* @file payloadbuffer.hpp
* @brief PayloadBufferクラスが定義されたヘッダ
* @author KDDI CORPORATION
* @details　PayloadBufferクラスが定義されたヘッダ。詳細な内容は各クラス、構造体、などを参照の事。
*/
#pragma once

#include "gpcclib/def.hpp"
#include "gpcclib/pointset.hpp"
#include "gpcclib/params.hpp"

/**
* @brief gpcclib名前空間
* @details GPCC SDKを使う場合に使用する名前空間
*/
namespace gpcclib {

//============================================================================
// Represents a frame in the encoder or decoder.
/**
* @brief 点群全体にかかわる情報を保持するクラス
* @details スケール情報や軸順序など点群全体にかかわる情報を保持するクラス。
*/
class GPCCLIB_INTERFACE_SPEC CloudFrame {
public:
  /**
  * @brief インスタンス生成
  * @details CloudFrameクラスのインスタンスを生成する
  * @return CloudFrame* 生成したCloudFrameインスタンスのポインタ
  */
  static CloudFrame* create();

  /**
  * @brief インスタンス生成
  * @details パラメータを指定してCloudFrameクラスのインスタンスを生成する
  * @param[in] sps: シーケンスパラメータ
  * @param[in] fixedPointBits:
  * @param[in] cloud:
  * @return CloudFrame* 生成したCloudFrameインスタンスのポインタ
  */
  static CloudFrame* create(
    const SequenceParameterSet& sps,
    int fixedPointBits,
    const PointSet3& cloud);

  /**
  * @brief インスタンス解放
  * @details CloudFrameクラスのインスタンスを解放する
  */
  virtual void release() = 0;

  /**
  * @brief フレーム番号を取得
  * @details フレーム番号を取得する
  * @return int フレーム番号
  */
  virtual int getFrameNum() const = 0;
  /**
  * @brief フレーム番号を設定
  * @details フレーム番号を設定する
  * @param[in] frameNum: フレーム番号
  */
  virtual void setFrameNum(int frameNum) = 0;

  /**
  * @brief 軸順序パターンを取得
  * @details 軸順序パターンを取得する
  * @return AxisOrder 軸順序パターン
  */
  virtual AxisOrder getGeometryAxisOrder() const = 0;

  /**
  * @brief 軸順序パターンを設定
  * @details 軸順序パターンを設定する
  * @param[in] order: 軸順序パターン
  */
  virtual void setGeometryAxisOrder(AxisOrder order) = 0;

  /**
  * @brief スケール長を取得
  * @details スケール長を取得する。
  * OutputUnitをkMetreに指定した場合メートル単位で扱います。
  * OutputUnitをkDimensionlessに指定した場合外部の座標単位で扱います。
  * @return double スケール長
  */
  virtual double getOutputUnitLength() const = 0;

  /**
  * @brief スケール長を設定
  * @details スケール長を設定する。
  * OutputUnitをkMetreに指定した場合メートル単位で扱います。
  * OutputUnitをkDimensionlessに指定した場合外部の座標単位で扱います。
  * @param[in] outputUnitLength スケール長
  */
  virtual void setOutputUnitLength(double outputUnitLength) = 0;

  /**
   * @brief スケール単位を取得
   * @details スケール単位を取得します。
   * @return ScaleUnit スケール単位
   */
  virtual ScaleUnit getOutputUnit() const = 0;

  /**
   * @brief スケール単位を設定
   * @details スケール単位を設定します。
   * @param[in] outputUnit スケール単位
   */
  virtual void setOutputUnit(ScaleUnit outputUnit) = 0;

  /**
   * @brief 原点座標を取得
   * @details 原点座標を取得します。
   * @return const point_t& 原点座標
   */
  virtual const point_t& getOutputOrigin() const = 0;

  /**
   * @brief 原点座標を取得
   * @details 原点座標を取得します。
   * @return point_t& 原点座標
   */
  virtual point_t& getOutputOrigin() = 0;

  /**
   * @brief 原点座標を設定
   * @details 原点座標を取得します。
   * @param[in] outputOrigin: 原点座標
   */
  virtual void setOutputOrigin(const point_t& outputOrigin) = 0;

  /**
   * @brief 少数ビットの数を取得
   * @details 少数ビットの数を取得します。
   * @return int 少数ビット数
   */
  virtual int getOutputFpBits() const = 0;

  /**
   * @brief 少数ビットの数を設定
   * @details 少数ビットの数を設定します。
   * @param[in] bits: 少数ビット数
   */
  virtual void setOutputFpBits(int bits) = 0;

  /**
   * @brief 属性詳細を取得
   * @details KnownAttributeLabelで指定した要素の属性詳細を返す。存在しない場合、 nullptr を返す。
   * @param[in] label: 取得したい要素
   * @return const AttributeDescription* 属性詳細のポインタ
   */
  virtual const AttributeDescription*
  getAttrDesc(KnownAttributeLabel label) const = 0;

  /**
   * @brief 属性詳細を取得
   * @details KnownAttributeLabelで指定した要素の属性詳細を返す。存在しない場合、 nullptr を返す。
   * @param[in] label: 取得したい要素
   * @return AttributeDescription* 属性詳細のポインタ
   */
  virtual AttributeDescription* getAttrDesc(KnownAttributeLabel label) = 0;

  /**
   * @brief 点群情報を取得
   * @details 点群情報を取得します。
   * @return const PointSet3& 点群情報の実体
   */
  virtual const PointSet3& getCloud() const = 0;
  /**
   * @brief 点群情報を取得
   * @details 点群情報を取得します。
   * @return const PointSet3& 点群情報の実体
   */
  virtual PointSet3& getCloud() = 0;

protected:
  /**
   * @brief CloudFrameクラスのコンストラクタ
   * @details CloudFrameクラスのデフォルトコンストラクタ指定
   */
  CloudFrame() = default;

  /**
   * @brief CloudFrameクラスのデストラクタ
   * @details CloudFrameクラスのデフォルトデストラクタ指定
   */
  virtual ~CloudFrame() = default;

private:
  /**
   * @brief CloudFrameクラスのコピーコンストラクタ
   * @details CloudFrameクラスのコピーコンストラクタを禁止します
   */
  CloudFrame(const CloudFrame&) = delete;
  /**
   * @brief CloudFrameクラスの代入演算子
   * @details CloudFrameクラスの代入演算子によるコピーを禁止します
   */
  CloudFrame& operator=(const CloudFrame&) = delete;
};

} // namespace gpcclib
