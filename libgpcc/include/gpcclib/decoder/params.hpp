/**
* @file decoder/params.hpp
* @brief Decoderクラスのパラメータが定義されたヘッダ
* @author KDDI CORPORATION
* @details Decoderクラスのパラメータが定義されたヘッダ。詳細な内容は各クラスメソッドなどを参照の事
*/
#pragma once

#include "gpcclib/def.hpp"

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
* @brief DecoderParamsクラス
* @details Decoderクラスで使用するパラメータクラス
*/
class GPCCLIB_INTERFACE_SPEC DecoderParams {
public:

  /**
  * @brief インスタンス生成
  * @details DecoderParamsクラスのインスタンスを生成する
  * @return DecoderParams* 生成したDecoderParamsインスタンスのポインタ
  */
  static DecoderParams* create();

  /**
  * @brief インスタンス解放
  * @details DecoderParamsクラスのインスタンスを解放する
  */
  virtual void release() = 0;
  
  /**
  * @brief  並列デコードする際のスレッド数を取得
  * @details 並列デコードする際のスレッド数を取得
  * @return 並列デコードする際のスレッド数
  */
  virtual unsigned int getNumOfFrameThreads() const = 0;
  
  /**
  * @brief  並列デコードする際のスレッド数を設定
  * @details フレーム単位で並列デコードする際のスレッド数を設定
  * @param[in] numOfThreads: 並列デコードする際のスレッド数
  */
  virtual void setNumOfFrameThreads(unsigned int numOfThreads) = 0;
	
  /**
  * @brief  並列デコードの実行可否を取得
  * @details 並列デコードの実行可否を取得
  * @return TRUE:並列デコード実行可　FALSE:並列デコード実行禁止
  */
  virtual bool doSliceParallelProc() const = 0;

  /**
  * @brief  並列デコードの実行可否を設定
  * @details 並列デコードの実行可否を設定
  * @param[in] doSliceParallel: TRUE:並列デコード実行可　FALSE:並列デコード実行禁止
  */
  virtual void setDoSliceParallelProc(bool doSliceParallel) = 0;

  /**
  * @brief  色空間の変換可否を取得
  * @details 色空間（デフォルトBT.709）への変換可否を取得
  * @return TRUE:変換可　FALSE:変換禁止
  */
  virtual bool shouldConvertColourspace() const = 0;

  /**
  * @brief  色空間の変換可否を設定
  * @details 色空間（デフォルトBT.709）への変換可否を設定
  * @param[in] enable: TRUE:変換可　FALSE:変換禁止
  */
  virtual void setConvertColourspace(bool enabled) = 0;

  // For partial decoding (aka, scalable bitstreams), the number of octree
  // layers to skip during the decode process (attribute coding must take
  // this into account)
  /**
  * @note TODO.確認中
  * @brief  オクトリーの部分デコードをスキップするレイヤ数を取得
  * @details オクトリーとアトリビュートの部分デコードをスキップするオクトリーレイヤ数を取得
  * @return 部分デコードをスキップするオクトリーレイヤ数を取得
  */
  virtual int  getMinGeomNodeSizeLog2() const = 0;

  /**
  * @note TODO.確認中
  * @brief  オクトリーの部分デコードをスキップするレイヤ数を設定
  * @details オクトリーとアトリビュートの部分デコードをスキップするオクトリーレイヤ数を設定。デフォルト0（フルデコード）
  * @param[in] minGeomNodeSizeLog2: 部分デコードをスキップするオクトリーレイヤ数を設定
  */
  virtual void setMinGeomNodeSizeLog2(int minGeomNodeSizeLog2) = 0;

  // A maximum number of points to partially decode.
  /**
  * @brief  部分デコードの最大ポイント数を取得
  * @details 部分デコードの最大ポイント数を取得
  * @return 部分デコードの最大ポイント数
  */
  virtual int  getDecodeMaxPoints() const = 0;

  /**
  * @brief  部分デコードの最大ポイント数を設定
  * @details 部分デコードの最大ポイント数を設定
  * @param[in] decodeMaxPoints: 部分デコードの最大ポイント数
  */
  virtual void setDecodeMaxPoints(int decodeMaxPoints) = 0;

  // Number of fractional bits used in output position representation.
  /**
  * @note TODO.調査中
  * @brief  出力形式を取得
  * @details 出力形式を取得 Number of fractional bits used in output position representation.
  * @return 0: intger -1:Auto(Full)
  */
  virtual int  getOutputFpBits() const = 0;
  
  /**
  * @note TODO.調査中
  * @brief  出力形式を設定
  * @details 出力形式を設定
  * @param[in] outputFpBits 0: intger -1:Auto(Full)
  */
  virtual void setOutputFpBits(int outputFpBits) = 0;

protected:
  DecoderParams() = default;
  virtual ~DecoderParams() = default;
  explicit DecoderParams(DecoderParams&&) noexcept = default;

private:
  DecoderParams(const DecoderParams&) = delete;
  DecoderParams& operator=(const DecoderParams&) = delete;
  DecoderParams& operator=(DecoderParams&&) noexcept = delete;
};

} // namespace decoder
} // namespace gpcclib
