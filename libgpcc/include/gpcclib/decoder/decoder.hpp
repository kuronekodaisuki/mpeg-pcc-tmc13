/**
* @file decoder/decoder.hpp
* @brief Decoderクラスが定義されたヘッダ
* @author KDDI CORPORATION
* @details Decoderクラスが定義されたヘッダ。詳細な内容は各クラスメソッドなどを参照の事
*/
#pragma once

#include "gpcclib/def.hpp"
#include "gpcclib/payloadbuffer.hpp"
#include "gpcclib/frame.hpp"
#include "gpcclib/decoder/params.hpp"

/**
* @brief gpcclib名前空間
* @details GPCC SDKを使う場合に使用する名前空間
*/
namespace gpcclib {

/**
* @brief decoder名前空間
* @details Decoderクラスを使う場合に使用する名前空間
*/
namespace decoder {

/**
* @brief Decoderクラス
* @details Decoderクラス。オブジェクト生成、リリース、デコード実行、結果の通知を行うメソッドを提供する
*/
class GPCCLIB_INTERFACE_SPEC Decoder {
public:
  /**
  * @brief デコードのコールバッククラス
  * @details デコード結果の通知を行うクラス
  */
  class Callbacks {
  public:
    
    /**
    * @brief デコード結果の通知
    * @details デコード結果の通知。CloudFrame型frameに結果を格納。getCloud()メソッドなどで取得可能
    * @param[in] frame: フレームデータ（CloudFrame型）
    */
    virtual void onOutputCloud(const CloudFrame& frame) = 0;
    virtual ~Callbacks() = default;

  protected:
    /**
    * @brief デフォルトコンストラクタ
    * @details Callbacksクラスのデフォルトコンストラクタ指定
    */
    Callbacks() = default;

  private:
    /**
    * @brief Callbacksクラスのコピーコンストラクタ
    * @details Callbacksクラスのコピーコンストラクタを禁止
    */
    Callbacks(const Callbacks&) = delete;

    /**
    * @brief Callbacksクラスの代入演算子
    * @details Callbacksクラスの代入演算子によるコピーを禁止
    */
    Callbacks& operator=(const Callbacks&) = delete;
  };
  
  /**
  * @brief インスタンス生成
  * @details Decoderクラスのインスタンスを生成する
  * @param[in] decParams: デコーダパラメータ(DecoderParams型)
  * @param[in] callbacks: コールバック
  * @return Decoder* 生成したDecoderインスタンスのポインタ
  */
  static Decoder* create(const DecoderParams& decParams, Callbacks& callbacks);
  
  /**
  * @brief インスタンス解放
  * @details Decoderクラスのインスタンスを解放する
  */
  virtual void release() = 0;
  
  /**
  * @brief デコード実施
  * @details デコード実施
  * @param[in] buf: 圧縮されたペイロード(PayloadBufferクラス)
  * @return -1: エラー発生　0: デコード成功　1: デコード失敗
  */
  virtual int decompress(const PayloadBuffer* buf) = 0;

protected:
  Decoder() = default;
  virtual ~Decoder() = default;

private:
  Decoder(const Decoder&) = delete;
  Decoder& operator=(const Decoder&) = delete;
};

} // namespace decoder
} // namespace gpcclib
