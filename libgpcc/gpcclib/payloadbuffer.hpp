/**
* @file payloadbuffer.hpp
* @brief PayloadBufferクラスが定義されたヘッダ
* @author KDDI CORPORATION
* @details　PayloadBufferクラスが定義されたヘッダ。詳細な内容は各クラス、構造体、などを参照の事。
*/
#pragma once

#include <cstddef>
#include "gpcclib/def.hpp"
#include "gpcclib/params.hpp"

/**
* @brief gpcclib名前空間
* @details GPCC SDKを使う場合に使用する名前空間
*/
namespace gpcclib {

/**
* @brief GPCC SDKで扱うデータを保持するクラス
* @details GPCC SDKで扱うデータを保持するクラス
*/
class GPCCLIB_INTERFACE_SPEC PayloadBuffer {
public:
  /**
  * @brief インスタンス生成
  * @details PayloadBufferクラスのインスタンスを生成
  * @return PayloadBuffer* 生成したPayloadBufferインスタンスのポインタ
  */
  static PayloadBuffer* create();

  /**
  * @brief インスタンス開放
  * @details PayloadBufferインスタンスを開放
  */
  virtual void release() = 0;

  /**
  * @brief データ種別取得
  * @details 保持しているデータ種別を取得する
  * @return PayloadType データ種別
  */
  virtual PayloadType getType() const = 0;

  /**
  * @brief データ種別設定
  * @details インスタンスが保持するデータ種別を設定する
  * @param [in] type:データ種別
  */
  virtual void setType(PayloadType type) = 0;

  /**
  * @brief バッファサイズ取得
  * @details 現在保持しているバッファサイズを取得する
  * @return size_t バッファサイズ
  */
  virtual size_t getSize() const = 0;

  /**
  * @brief バッファポインタの取得
  * @details 保持しているデータにアクセスするためのバッファポインタを取得する
  * @return const char* バッファポインタ
  */
  virtual const char* getBuffer() const = 0;

  /**
  * @brief バッファポインタの取得
  * @details 保持しているデータにアクセスするためのバッファポインタを取得する
  * @return char* バッファポインタ
  */
  virtual char* getBuffer() = 0;

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

protected:
  /**
   * @brief PayloadBufferクラスのコンストラクタ
   * @details PayloadBufferクラスのデフォルトコンストラクタ指定
   */
  PayloadBuffer() = default;

  /**
   * @brief PayloadBufferクラスのデストラクタ
   * @details PayloadBufferクラスのデフォルトデストラクタ指定
   */
  virtual ~PayloadBuffer() = default;

  /**
   * @brief PayloadBufferクラスの移動コンストラクタ
   * @details 別のインスタンスから移動してあらたなインスタンスを生成します。また、例外をスローしません。
   */
  explicit PayloadBuffer(PayloadBuffer&&) noexcept = default;

private:
  /**
   * @brief PayloadBufferクラスのコピーコンストラクタ
   * @details PayloadBufferクラスのコピーコンストラクタを禁止します
   */
  PayloadBuffer(const PayloadBuffer&) = delete;

  /**
   * @brief PayloadBufferクラスの代入演算子
   * @details PayloadBufferクラスの代入演算子によるコピーを禁止します
   */
  PayloadBuffer& operator=(const PayloadBuffer&) = delete;

  /**
   * @brief PayloadBufferクラスの代入演算子
   * @details PayloadBufferクラスの代入演算子による移動を禁止します
   */
  PayloadBuffer& operator=(const PayloadBuffer&&) = delete;
};

} // namespace gpcclib
