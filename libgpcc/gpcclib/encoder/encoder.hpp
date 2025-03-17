/**
* @file encoder/encoder.hpp
* @brief Encoderクラスが定義されたヘッダ
* @author KDDI CORPORATION
* @details Encoderクラスが定義されたヘッダ。詳細な内容は各クラスメソッドなどを参照の事
*/
#pragma once

#include "gpcclib/def.hpp"
#include "gpcclib/payloadbuffer.hpp"
#include "gpcclib/pointset.hpp"
#include "gpcclib/frame.hpp"
#include "gpcclib/encoder/params.hpp"

/**
* @brief gpcclib名前空間
* @details GPCC SDKを使う場合に使用する名前空間
*/
namespace gpcclib {

/**
* @brief encoder名前空間
* @details Encoderクラスを使う場合に使用する名前空間
*/
namespace encoder {

/**
* @brief Encoderクラス
* @details Encoderクラス。オブジェクト生成、リリース、デコード実行、結果の通知を行うメソッドを提供する
*/
class GPCCLIB_INTERFACE_SPEC Encoder {
public:
  
  /**
  * @brief エンコードのコールバッククラス
  * @details エンコード結果の通知を行うクラス
  */
  class Callbacks {
  public:
    /**
    * @brief エンコードデータの書き出し
    * @details エンコードデータをTLV形式でcompressedStreamPathで指定されたファイルに書き出し
    * @param[in] buf: エンコードデータ
    */
    virtual void onOutputBuffer(const PayloadBuffer& buf) = 0;
    
    /**
    * @note TODO.調査中
    * @brief 再構築データのplyファイル書き出し
    * @details 再構築データのplyファイルのpostRecolorPathへの書き出し
    * @param[in] frame: 読み込んだフレームデータ（CloudFrame型）
    */
    virtual void onPostRecolour(const CloudFrame& frame) = 0;
/**
    * @note TODO.調査中
    * @brief 再構築データのデータパスが存在するか
    * @details 再構築データのデータパスreconstructedDataPathが存在するか
    * @return TRUE:存在　FALSE:存在しない
    */
    virtual bool needsReconstructedCloud() const = 0;

    /**
    * @brief 再構築データのplyファイル書き出し
    * @details 再構築データのplyファイルのreconstructedDataPathへの書き出し
    * @param[in] frame: 読み込んだフレームデータ（CloudFrame型）
    */    
    virtual void
    onReconstructedCloud(const CloudFrame& reconstructedFrame) = 0;
    virtual ~Callbacks() = default;

  protected:
    Callbacks() = default;

  private:
    Callbacks(const Callbacks&) = delete;
    Callbacks& operator=(const Callbacks&) = delete;
  };

  /**
  * @brief インスタンス生成
  * @details Encoderクラスのインスタンスを生成する
  * @param[in] encParams: デコーダパラメータ(EncoderParams型)
  * @param[in] callbacks: コールバック
  * @return Encoder* 生成したEncoderインスタンスのポインタ
  */
  static Encoder* create(const EncoderParams& encParams, Callbacks& callbacks);

  /**
  * @brief インスタンス解放
  * @details Encoderクラスのインスタンスを解放する
  */
  virtual void release() = 0;
  
  /**
  * @brief エンコード実施
  * @details エンコード実施
  * @param[in] inputPointCloud: 点群データ(PointSet3型ポインタ)
  * @return -1: エラー発生　0: デコード成功　1: デコード失敗
  */
  virtual int compress(const PointSet3* inputPointCloud) = 0;

protected:
  Encoder() = default;
  virtual ~Encoder() = default;

private:
  Encoder(const Encoder&) = delete;
  Encoder& operator=(const Encoder&) = delete;
};

} // namespace encoder
} // namespace gpcclib
