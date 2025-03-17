/**
* @file params.hpp
* @brief GPCCコーデックに設定するパラメータが記載されたヘッダ
* @author KDDI CORPORATION
* @details GPCCコーデックに設定するパラメータが記載されたヘッダ。詳細な内容は各クラス、構造体、などを参照の事。
*/

#pragma once

#include "gpcclib/def.hpp"
#include "gpcclib/common.hpp"

/**
* @brief gpcclib名前空間
* @details GPCC SDKを使う場合に使用する名前空間
*/
namespace gpcclib {

//============================================================================

/**
* @brief ペイロードタイプ
* @details 使用するGPCCペイロードタイプ列挙
*/
enum class PayloadType
{
  //! ペイロードタイプ：シーケンスパラメータ(SequenceParameter)
  kSequenceParameterSet = 0,
  //! ペイロードタイプ：ジオメトリーパラメータ(GeometryParameter)
  kGeometryParameterSet = 1,
  //! ペイロードタイプ：ジオメトリーブリック(GeometryBrick)
  kGeometryBrick = 2,
  //! ペイロードタイプ：アトリビュートパラメータ(AttributeParameter)
  kAttributeParameterSet = 3,
  //! ペイロードタイプ：アトリビュートブリック(AttributeBrick)
  kAttributeBrick = 4,
  //! ペイロードタイプ：タイルインベントリ(TileInventory)
  kTileInventory = 5,
  //! ペイロードタイプ：フレームバウンダリーマーカー(FrameBoundaryMarker)
  kFrameBoundaryMarker = 6,
  //! ペイロードタイプ：コンスタントアトリビュート(ConstantAttribute)
  kConstantAttribute = 7,
  //! ペイロードタイプ：ジェネライズドアトリビュートインベントリ(GeneralizedAttrParamInventory)
  kGeneralizedAttrParamInventory = 8,
  //! ペイロードタイプ：ユーザーデータ(UserData)
  kUserData = 9,
  //! ペイロードタイプ：アンノウンペイロード(UnknownPayload)
  kUnknownPayload = -1,
};

//============================================================================

/**
* @brief アトリビュートラベル
* @details アトリビュートラベル列挙
*/
enum class KnownAttributeLabel : uint32_t
{
  //! カラー
  kColour = 0,
  //! 反射率
  kReflectance = 1,
  //! 不透明度
  kOpacity = 2,
  //! フレームインデックス
  kFrameIndex = 3,
  //! フレーム番号
  kFrameNumber = 4,
  //!　マテリアルID
  kMaterialId = 5,
  //! ノーマル
  kNormal = 6,

  // Indicates that the attrabute label is described by an Oid
  //! オブジェクト識別番号 :アトリビュートラベルがオブジェクト識別番号(Oid)で記述されていることを示す。
  kOid = 0xffffffff,
};

//============================================================================

/**
* @brief Oidクラス
* @details オブジェクト識別番号（Oid）クラス
*/
class GPCCLIB_INTERFACE_SPEC Oid {
public:
  /**
  * @brief インスタンス生成
  * @details Oidクラスのインスタンスを生成する
  * @return Oid* 生成したインスタンスのポインタ
  */
  static Oid* create();

  /**
  * @brief インスタンス生成
  * @details Oidクラスのインスタンスを生成する
  * @param[in] contents: コンテンツ
  * @param[in] contentSize: コンテンツサイズ
  * @return Oid* 生成したインスタンスのポインタ
  */
  static Oid* create(const uint8_t* contents, size_t contentsSize);

  /**
  * @brief インスタンス削除
  * @details Oidクラスのインスタンスを削除する
  */
  virtual void release() = 0;

  /**
  * @brief コンテンツを取得
  * @details Oidクラスのコンテンツを取得する
  * @return UInt8VectorAdapter&: Oidクラスに保存されたコンテンツ
  */
  virtual const UInt8VectorAdapter& getConents() const = 0;

  /**
  * @brief コンテンツを取得
  * @details Oidクラスのコンテンツを取得する
  * @return UInt8VectorAdapter&: Oidクラスに保存されたコンテンツ
  */
  virtual UInt8VectorAdapter& getConents() = 0;

protected:
  Oid() = default;
  virtual ~Oid() = default;
  explicit Oid(Oid&&) noexcept = default;

private:
  Oid(const Oid&) = delete;
  Oid& operator=(const Oid&) = delete;
  Oid& operator=(Oid&&) noexcept = delete;
};

//============================================================================

/**
* @brief アトリビュートラベルクラス
* @details アトリビュートラベルクラス。KnownAttributeLabel列挙参照
*/
class GPCCLIB_INTERFACE_SPEC AttributeLabel {
public:
  /**
  * @brief インスタンス生成
  * @details アトリビュートラベルクラスのインスタンスを生成する
  * @return AttributeLabel*: 生成したインスタンスのポインタ
  */
  static AttributeLabel* create();

  /**
  * @brief インスタンス生成
  * @details アトリビュートラベルクラスのインスタンスをknown_attribute_labelで生成する
  * @param[in] known_attribute_label: アトリビュートラベル
  * @return AttributeLabel*: 生成したインスタンスのポインタ
  */
  static AttributeLabel* create(KnownAttributeLabel known_attribute_label);

  /**
  * @brief インスタンス削除
  * @details アトリビュートラベルクラスのインスタンスを削除する
  */
  virtual void release() = 0;

  // KnownAttributeLabel known_attribute_label;
  /**
  * @brief アトリビュートラベルを取得
  * @details 保存されたアトリビュートラベルを取得する
  * @return 保存されたアトリビュートラベル
  */
  virtual KnownAttributeLabel getKnownAttributeLabel() const = 0;

  /**
  * @brief アトリビュートラベルを設定
  * @details アトリビュートラベルを設定する
  * @return 保存されたアトリビュートラベル
  */
  virtual void setKnownAttributeLabel(KnownAttributeLabel label) = 0;

  // Oid oid;
  /**
  * @brief オブジェクト識別番号(Oid)を取得
  * @details オブジェクト識別番号(Oid)を取得
  * @return オブジェクト識別番号(Oid)
  */
  virtual const Oid& getOid() const = 0;

  /**
  * @brief オブジェクト識別番号(Oid)を取得
  * @details オブジェクト識別番号(Oid)を取得
  * @return オブジェクト識別番号(Oid)
  */
  virtual Oid& getOid() = 0;

protected:
  AttributeLabel() = default;
  virtual ~AttributeLabel() = default;
  explicit AttributeLabel(AttributeLabel&&) noexcept = default;

private:
  AttributeLabel(const AttributeLabel&) = delete;
  AttributeLabel& operator=(const AttributeLabel&) = delete;
  AttributeLabel& operator=(AttributeLabel&&) noexcept = delete;
};

//============================================================================

/**
 * @brief 点群の圧縮属性
 * @details RAHT は八分木で分割した空間を空間全体から再帰的にたどり，
 * 八分木の下位のレベルにあるノードの色を基に次のレベルにあるノードの色を予測することで圧縮する．
 * Predicting Transform では最初に与えられた点群を粗い粒度から細かい粒度へ便宜的に分類し，
 * 細かい粒度の属性は粗い粒度の属性から平均値か近傍値という予測方法を適用することで
 * 細かい粒度から粗い粒度の順に属性を圧縮する．
 * Lifting Transform は Predicting Transform を基にしているが細かい粒度から粗い粒度に圧縮する中で，
 * 近傍点との距離などを変数に粒度ごとの属性値を更新する手順が追加されている
 */
enum class AttributeEncoding
{
  //! Regional Adaptive Hierarchical Transform（RAHT）
  kRAHTransform = 0,
  //! Predicting Transform
  kPredictingTransform = 1,
  //! Lifting Transform
  kLiftingTransform = 2,
  //! 非圧縮
  kRaw = 3,
};

//============================================================================

/**
 * @brief LoDサンプリング種別
 * @details Predicting/Lifting Transform時のサンプリング種別
 */
enum class LodDecimationMethod
{
  //! 無し
  kNone = 0,
  //! 周期
  kPeriodic = 1,
  //! 中心
  kCentroid = 2,
};

//============================================================================

/**
 * @brief 軸順序パターン
 * @details 軸の組み合わせによる順序のパターン
 */
enum class AxisOrder
{
  //! ZYX
  kZYX = 0,
  //! XYZ
  kXYZ = 1,
  //! XZY
  kXZY = 2,
  //! KYZX
  kYZX = 3,
  //! ZYX
  kZYX_4 = 4,
  //! ZXY
  kZXY = 5,
  //! YXZ
  kYXZ = 6,
  //! XYZ
  kXYZ_7 = 7,
};

//============================================================================
// ISO/IEC 23001-8 codec independent code points
/**
* @brief カラーマトリックス
* @details ISO/IEC 23001-8 Codec Independent Code Pointsで定義された色種別
* に従ってコード化された属性値の色空間を示す
* convertPlyColourspace=1`と併用すると、エンコーダーとデコーダーの入出力で色空間変換が行われる
* 
*  | Value | RGB converter | Description                               |
*  |:-----:|:-------------:|------------------------------------------ |
*  | 0     | n/a           | Direct coding (eg, RGB, XYZ)              |
*  | 1     | Yes           | YCbCr ITU-R BT.709                        |
*  | 2     | n/a           | Unspecified                               |
*  | 3     | n/a           | Reserved                                  |
*  | 4     | No            | USA Title 47 CFR 73.682 (a)(20)           |
*  | 5     | No            | YCbCr ITU-R BT.601                        |
*  | 6     | No            | YCbCr SMPTE 170M                          |
*  | 7     | No            | YCbCr SMPTE 240M                          |
*  | 8     | Yes (YCgCoR)  | YCgCo / YCgCoR                            |
*  | 9     | No            | YCbCr ITU-R BT.2020                       |
*  | 10    | No            | YCbCr ITU-R BT.2020 (constant luminance)  |
*  | 11    | No            | YDzDx SMPTE ST 2085                       |
*/
enum class ColourMatrix : uint8_t
{
  kIdentity = 0,
  kBt709 = 1,
  kUnspecified = 2,
  kReserved_3 = 3,
  kUsa47Cfr73dot682a20 = 4,
  kBt601 = 5,
  kSmpte170M = 6,
  kSmpte240M = 7,
  kYCgCo = 8,
  kBt2020Ncl = 9,
  kBt2020Cl = 10,
  kSmpte2085 = 11,
};

//============================================================================
/**
* @brief アトリビュートパラメータタイプ
* @details アトリビュートパラメータのデータタイプを示す
* 符号化されたアトリビュート値を解釈するためにスケールとオフセットは用いられる
* エンコーダはコード化された属性値を $(attr - offset) / scale$ として導出する。
* エンコーダーとデコーダーはコード化された属性を $attr × scale + offset$ の
* ようにスケーリングして出力する。
*/
enum class AttributeParameterType : uint8_t
{
  //! ITU-T35
  kItuT35 = 0,
  //! オブジェクト識別子
  kOid = 1,
  //! ISO/IEC 23001-8:2016 Coding-Independent Code Points
  kCicp = 2,
  //! スケーリング値
  kScaling = 3,
  //! デフォルト値
  kDefaultValue = 4,
  /* [3, 127] are reserved for future use */
  /* [128, 255] are specified according to the attribute label */
};

//============================================================================
/**
* @note TODO.調査中(2) ふさわしい表現
* @brief オペークアトリビュートクラス
* @details 各種情報を格納したクラス
* アトリビュートパラメータタイプごとに以下パラメータを格納する
　　　0: カントリーコード、拡張カントリーコード
　　　1: オブジェクト識別子
　　　0,1以外: 個別データ（8bitベクタ列）
*/
class GPCCLIB_INTERFACE_SPEC OpaqueAttributeParameter {
public:
  
  /**
  * @brief インスタンス生成
  * @details オペークアトリビュートクラスのインスタンスを生成する
  * @return OpaqueAttributeParameter*: 生成したオペークアトリビュートクラスの
  * インスタンスのポインタ
  */
  static OpaqueAttributeParameter* create();

  /**
  * @brief インスタンス削除
  * @details オペークアトリビュートクラスのインスタンスを削除する
  */
  virtual void release() = 0;

  // AttributeParameterType attr_param_type;
  /**
  * @brief 格納されているアトリビュートパラメータタイプを取得する
  * @details 格納されているアトリビュートパラメータタイプを取得する
  * @return AttributeParameterType: アトリビュートパラメータタイプ
  */
  virtual AttributeParameterType getAttrParamType() const = 0;
  
  /**
  * @brief アトリビュートパラメータタイプを設定する
  * @details アトリビュートパラメータタイプを設定する
  * @param[in] type: アトリビュートパラメータタイプ
  */
  virtual void setAttrParamType(AttributeParameterType type) = 0;

  // int attr_param_itu_t_t35_country_code;
  // int attr_param_itu_t_t35_country_code_extension;
  /**
  * @brief カントリーコードを取得する
  * @details ITU-T勧告T.35定義のカントリーコードを取得する
  * @return int: カントリーコード
  */
  virtual int getAttrParamItuTT35CountryCode() const = 0;
  /**
  * @brief カントリーコードを設定する
  * @details ITU-T勧告T.35定義のカントリーコードを設定する
  * @param[in] code: カントリーコード
  */
  virtual void setAttrParamItuTT35CountryCode(int code) = 0;

  /**
  * @brief 拡張カントリーコードを取得する
  * @details ITU-T勧告T.35定義の拡張カントリーコードを取得する
  * @return int: 拡張カントリーコード
  */
  virtual int getAttrParamItuTT35CountryCodeExtension() const = 0;
  /**
  * @brief 拡張カントリーコードを設定する
  * @details ITU-T勧告T.35定義の拡張カントリーコードを設定する
  * @param[in] val: 拡張カントリーコード
  */
  virtual void setAttrParamItuTT35CountryCodeExtension(int val) = 0;

  //Oid attr_param_oid;
  /**
  * @brief オブジェクト識別子を取得する
  * @details 本オブジェクトの識別子を取得する
  * @return Oid&: オブジェクト識別子
  */
  virtual const Oid& getAttrParamOid() const = 0;

   /**
  * @brief オブジェクト識別子を取得する
  * @details 本オブジェクトの識別子を取得する
  * @return Oid&: オブジェクト識別子
  */
  virtual Oid& getAttrParamOid() = 0;

  // std::vector<uint8_t> attr_param_byte;
  /**
  * @brief タイプtype0/type1以外のアトリビュートデータ
  * @details アトリビュートパラメータタイプがtype0/type1以外のアトリビュートデータベクタ
  * @return UInt8VectorAdapter&: アトリビュートデータベクタ
  */
  virtual const UInt8VectorAdapter& getAttrParamByte() const = 0;

  /**
  * @brief タイプtype0/type1以外のアトリビュートデータ
  * @details アトリビュートパラメータタイプがtype0/type1以外のアトリビュートデータベクタ
  * @return UInt8VectorAdapter&: アトリビュートデータベクタ
  */
  virtual UInt8VectorAdapter& getAttrParamByte() = 0;

protected:
  OpaqueAttributeParameter() = default;
  virtual ~OpaqueAttributeParameter() = default;
  explicit OpaqueAttributeParameter(OpaqueAttributeParameter&&) noexcept =
    default;

private:
  OpaqueAttributeParameter(const OpaqueAttributeParameter&) = delete;
  OpaqueAttributeParameter&
  operator=(const OpaqueAttributeParameter&) = delete;
  OpaqueAttributeParameter&
  operator=(OpaqueAttributeParameter&&) noexcept = delete;
};

//============================================================================
/**
* @brief OpaqueAttributeParameterVector定義
* @details OpaqueAttributeParameterクラスのベクタ列としてOpaqueAttributeParameterVectorを定義
*/
typedef ObjectVectorAdapter<OpaqueAttributeParameter>
  OpaqueAttributeParameterVector;

/**
* @brief アトリビュートパラメータクラス
* @details アトリビュートパラメータを格納したクラス
*/
class GPCCLIB_INTERFACE_SPEC AttributeParameters {
public:

  /**
  * @brief インスタンス生成
  * @details AttributeParametersクラスのインスタンスを生成する
  * @return AttributeParameters*: 生成したAttributeParametersインスタンスのポインタ
  */
  static AttributeParameters* create();

  /**
  * @brief インスタンス解放
  * @details AttributeParametersクラスのインスタンスを解放する
  */
  virtual void release() = 0;

  // bool cicpParametersPresent;
  /**
  * @brief CICP(Coding-independent code points)パラメータが存在するかを取得
  * @details 本値はCICP(Coding-independent code points)パラメータが存在するかを示す。
  * @return bool: TRUE: CICPパラメータが存在
  * @return bool: FALSE: CICPパラメータが存在しない
  */
  virtual bool getCicpParametersPresent() const = 0;

  /**
  * @brief CICP(Coding-independent code points)パラメータが存在するかを設定
  * @details 本値はCICP(Coding-independent code points)パラメータが存在するかを示す。
  * @param[in] bool val: TRUE: CICPパラメータが存在
  * @param[in] bool val: FALSE: CICPパラメータが存在しない
  */
  virtual void setCicpParametersPresent(bool val) = 0;

  // int cicp_colour_primaries_idx;
  /**
  * @brief CICPのColorPrimariesパラメータを取得
  * @details 本値はCICPのColorPrimariesパラメータを示します。
  * @return int: CICPのColorPrimariesパラメータ
  */
  virtual int getCicpColourPrimariesIdx() const = 0;

  /**
  * @brief CICPのColorPrimariesパラメータを設定
  * @details 本値はCICPのColorPrimariesパラメータを示します。
  * @param[in] int idx: CICPのColorPrimariesパラメータ
  */
  virtual void setCicpColourPrimariesIdx(int idx) = 0;

  // int cicp_transfer_characteristics_idx;
  /**
  * @brief CICPのTransferCharacteristicsパラメータを取得
  * @details 本値はCICPのTransferCharacteristicsパラメータを示します。
  * @return int: CICPのTransferCharacteristicsパラメータ
  */
  virtual int getCicpTransferCharacteristicsIdx() const = 0;
  
  /**
  * @brief CICPのTransferCharacteristicsパラメータを設定
  * @details 本値はCICPのTransferCharacteristicsパラメータを示します。
  * @param[in] int val: CICPのTransferCharacteristicsパラメータ
  */
  virtual void setCicpTransferCharacteristicsIdx(int val) = 0;

  // ColourMatrix cicp_matrix_coefficients_idx;
  /**
  * @brief CICPのMatrixCoefficientsパラメータを取得
  * @details 本値はCICPのMatrixCoefficientsパラメータを示します。
  * @return ColourMatrix: CICPのMatrixCoefficientsパラメータ
  */
  virtual ColourMatrix getCicpMatrixCoefficientsIdx() const = 0;
  
  /**
  * @brief CICPのMatrixCoefficientsパラメータを設定
  * @details 本値はCICPのMatrixCoefficientsパラメータを示します。
  * @param[in] ColourMatrix idx: CICPのMatrixCoefficientsパラメータ
  */
  virtual void setCicpMatrixCoefficientsIdx(ColourMatrix idx) = 0;

  // bool cicp_video_full_range_flag;
  /**
  * @brief CICPのVideoFullRangeFlagパラメータを取得
  * @details 本値はCICPのVideoFullRangeFlagパラメータを示します。
  * @return bool: TRUE: CICPのVideoFullRangeFlagが有効（Full Range）
  * @return bool: FALSE: CICPのVideoFullRangeFlagが無効（Narrow Range）
  */
  virtual bool getCicpVideoFullRangeFlag() const = 0;

  /**
  * @brief CICPのVideoFullRangeFlagパラメータを設定
  * @details 本値はCICPのVideoFullRangeFlagパラメータを示します。
  * @param[in] bool flag: TRUE: CICPのVideoFullRangeFlagが有効（Full Range）
  * @param[in] bool flag: FALSE: CICPのVideoFullRangeFlagが無効（Narrow Range）
  */
  virtual void setCicpVideoFullRangeFlag(bool flag) = 0;

  // bool scalingParametersPresent;
  /**
  * @brief スケーリングパラメータの有無を取得
  * @details 本値はスケーリングパラメータの有無を示します。
  * @return bool : TRUE: スケーリングパラメータ有
  * @return bool : FALSE: スケーリングパラメータ無
  */
  virtual bool getScalingParametersPresent() const = 0;

  /**
  * @brief スケーリングパラメータの有無を設定
  * @details 本値はスケーリングパラメータの有無を示します。
  * @param[in] bool val: TRUE: スケーリングパラメータ有
  * @param[in] bool val: FALSE: スケーリングパラメータ無
  */
  virtual void setScalingParametersPresent(bool val) = 0;

  // int attr_scale_minus1;
  /**
  * @brief アトリビュートのスケールを取得
  * @details 本値はアトリビュートのスケール値を示します。
  * スケール値、端数ビット、オフセットと組み合わせてアトリビュート値を表します。
  * @return int: アトリビュートのスケール値
  */
  virtual int getAttrScaleMinus1() const = 0;

  /**
  * @brief アトリビュートのスケールを設定
  * @details 本値はアトリビュートのスケール値を示します。
  * スケール値、端数ビット、オフセットと組み合わせてアトリビュート値を表します。
  * @param[in] int val: アトリビュートのスケール値
  */
  virtual void setAttrScaleMinus1(int val) = 0;

  // int attr_frac_bits;
  /**
  * @brief アトリビュートの端数ビット数を取得
  * @details 本値はアトリビュートの端数ビットを示します。
  * スケール値、端数ビット、オフセットと組み合わせてアトリビュート値を表します。
  * @return int: アトリビュートのビット数
  */
  virtual int getAttrFracBits() const = 0;

  /**
  * @brief アトリビュートの端数ビット数を設定
  * @details 本値はアトリビュートの端数ビット数を示します。
  * スケール値、端数ビット、オフセットと組み合わせてアトリビュート値を表します。
  * @param[in] int bits: アトリビュートのビット数
  */
  virtual void setAttrFracBits(int bits) = 0;

  // int attr_offset;
  /**
  * @brief アトリビュートのオフセット値を取得
  * @details 本値はアトリビュートのオフセット値を示します。
  * スケール値、端数ビット、オフセットと組み合わせてアトリビュート値を表します。
  * @return int: アトリビュートのオフセット値
  */
  virtual int getAttrOffset() const = 0;

  /**
  * @brief アトリビュートのオフセット値を設定
  * @details 本値はアトリビュートのオフセット値を示します。
  * スケール値、端数ビット、オフセットと組み合わせてアトリビュート値を表します。
  * @param[in] int offset: アトリビュートのオフセット値
  */
  virtual void setAttrOffset(int offset) = 0;

  // std::vector<int> attr_default_value;
  /**
  * @brief アトリビュートのデフォルト値を取得
  * @details 保存されているアトリビュートのデフォルト値を示します。
  * @return Int32VectorAdapter&: アトリビュートのオフセット値
  */
  virtual const Int32VectorAdapter& getAttrDefaultValue() const = 0;

  /**
  * @brief アトリビュートのデフォルト値を取得
  * @details 保存されているアトリビュートのデフォルト値を示します。
  * @return Int32VectorAdapter&: アトリビュートのオフセット値
  */
  virtual Int32VectorAdapter& getAttrDefaultValue() = 0;

  // std::vector<OpaqueAttributeParameter> opaqueParameters;
  /**
  * @brief オペークアトリビュートパラメータを取得
  * @details オペークアトリビュートパラメータを取得します。
  * パラメータ内容はオペークアトリビュートクラスを参照下さい。
  * @return OpaqueAttributeParameterVector&: オペークアトリビュートパラメータ
  */
  virtual const OpaqueAttributeParameterVector&
  getOpaqueParameters() const = 0;
  
  /**
  * @brief オペークアトリビュートパラメータを取得
  * @details オペークアトリビュートパラメータを取得します。
  * パラメータ内容はオペークアトリビュートクラスを参照下さい。
  * @return OpaqueAttributeParameterVector&: オペークアトリビュートパラメータ
  */
  virtual OpaqueAttributeParameterVector& getOpaqueParameters() = 0;
  
  /**
  * @brief アトリビュートパラメータの内容をクリア
  * @details アトリビュートパラメータの内容を全てクリアします。
  * CICPパラメータ、スケーリングパラメータ、保存されたアトリビュート、オペークパラメータ
  */
  virtual void clear() = 0;

protected:
  AttributeParameters() = default;
  virtual ~AttributeParameters() = default;
  explicit AttributeParameters(AttributeParameters&&) noexcept = default;

private:
  AttributeParameters(const AttributeParameters&) = delete;
  AttributeParameters& operator=(const AttributeParameters&) = delete;
  AttributeParameters& operator=(AttributeParameters&&) noexcept = delete;
};

//============================================================================
/**
* @brief アトリビュートディスクリプションクラス
* @details アトリビュートの次元数、ID、ラベル、アトリビュートパラメータを格納したクラス
*/
class GPCCLIB_INTERFACE_SPEC AttributeDescription {
public:
  /**
  * @brief インスタンス生成
  * @details AttributeDescriptionクラスのインスタンスを生成する
  * @return AttributeDescription*: 生成したAttributeDescriptionインスタンスのポインタ
  */
  static AttributeDescription* create();
  
  /**
  * @brief インスタンス解放
  * @details AttributeDescriptionクラスのインスタンスを解放する
  */
  virtual void release() = 0;

  // int attr_num_dimensions_minus1;
  /**
  * @brief アトリビュートの次元数を取得
  * @details 本値はアトリビュートの次元数を示します。
  * 例）色：3-1=2
  * 　　Reflectance：1-1=0
  * @return int: アトリビュートの次元数
  */
  virtual int getAttrNumDimensionsMinus1() const = 0;

  /**
  * @brief アトリビュートの次元数を設定
  * @details 本値はアトリビュートの次元数を示します。
  * 例）色：3-1=2
  * 　　Reflectance：1-1=0
  * @param[in] int num: アトリビュートの次元数
  */
  virtual void setAttrNumDimensionsMinus1(int num) = 0;

  // int attr_instance_id;
  /**
  * @brief アトリビュートのインスタンスIDを取得
  * @details 本値はアトリビュートのインスタンスIDを示します。
  * @return int: アトリビュートのインスタンスID
  */
  virtual int getAttrInstanceId() const = 0;

  /**
  * @brief アトリビュートのインスタンスIDを設定
  * @details 本値はアトリビュートのインスタンスIDを示します。
  * @param[in] int id: アトリビュートのインスタンスID
  */
  virtual void setAttrInstanceId(int id) = 0;

  // int bitdepth;
  /**
  * @brief アトリビュートのビット幅を取得
  * @details 本値はアトリビュートのビット幅を示します。
  * @return int: アトリビュートのビット幅
  */
  virtual int getBitDepth() const = 0;

  /**
  * @brief アトリビュートのビット幅を設定
  * @details 本値はアトリビュートのビット幅を示します。
  * @param[in] int bitdepth: アトリビュートのビット幅
  */
  virtual void setBitDepth(int bitdepth) = 0;

  // AttributeLabel attributeLabel;
  /**
  * @brief アトリビュートのラベルを取得
  * @details 本値はアトリビュートのラベルを示します。
  * @return AttributeLabel&: アトリビュートのラベル
  */
  virtual const AttributeLabel& getAttributeLabel() const = 0;

  /**
  * @brief アトリビュートのラベルを取得
  * @details 本値はアトリビュートのラベルを示します。
  * @return AttributeLabel&: アトリビュートのラベル
  */
  virtual AttributeLabel& getAttributeLabel() = 0;

  // AttributeParameters params;
  /**
  * @brief アトリビュートパラメータを取得
  * @details 格納されているアトリビュートパラメータを取得を示します。
  * @return AttributeParameters&: アトリビュートパラメータ
  */
  virtual const AttributeParameters& getParams() const = 0;

  /**
  * @brief アトリビュートパラメータを取得
  * @details 格納されているアトリビュートパラメータを取得を示します。
  * @return AttributeParameters&: アトリビュートパラメータ
  */
  virtual AttributeParameters& getParams() = 0;

protected:
  AttributeDescription() = default;
  virtual ~AttributeDescription() = default;
  explicit AttributeDescription(AttributeDescription&&) noexcept = default;

private:
  AttributeDescription(const AttributeDescription&) = delete;
  AttributeDescription& operator=(const AttributeDescription&) = delete;
  AttributeDescription& operator=(AttributeDescription&&) noexcept = delete;
};

//============================================================================
/**
* @brief プロファイルコンパチビリティクラス
* @details プロファイル適合性を格納したクラス。プロファイル内容は以下
* 　・メインプロファイルと適合しているか
* 　・スライスが再分割されたときにビットストリームを切断するか
* 　・再構築フレームに重複点がないか
*/
class GPCCLIB_INTERFACE_SPEC ProfileCompatibility {
public:
  
  /**
  * @brief インスタンス生成
  * @details ProfileCompatibilityクラスのインスタンスを生成する
  * @return ProfileCompatibility* 生成したProfileCompatibilityインスタンスのポインタ
  */
  static ProfileCompatibility* create();

  /**
  * @brief インスタンス削除
  * @details ProfileCompatibilityクラスのインスタンスを削除する
  */
  virtual void release() = 0;

  // bool main_profile_compatibility_flag;
  /**
  * @brief メインプロファイルと適合性あるか取得
  * @details メインプロファイルと適合性あるか取得
  * @return TRUE:メインプロファイルと適合　FALSE:メインプロファイルと適合しない
  */
  virtual bool getMainProfileCompatiblityFlag() const = 0;
  
  /**
  * @brief メインプロファイルと適合性あるか設定
  * @details メインプロファイルと適合性あるか設定
  * @param[in] compatible: TRUE:メインプロファイルと適合　FALSE:メインプロファイルと適合しない
  */
  virtual void setMainProfileCompatiblityFlag(bool compatible) = 0;

  // int reserved_profile_compatibility_21bits;
  /**
  * @note TODO.調査中(2)
  * @brief reserved_profile_compatibility_21bits取得
  * @details reserved for future use
  * @return int: reserved_profile_compatibility_21bits
  */
  virtual int getReservedProfileCompatibility21Bits() const = 0;
  
  /**
  * @note TODO.調査中(2)
  * @brief reserved_profile_compatibility_21bits設定
  * @details reserved for future use
  * @param[in] bits: reserved_profile_compatibility_21bits
  */
  virtual void setReservedProfileCompatibility21Bits(int bits) = 0;

  // bool slice_reordering_constraint_flag;
  /**
  * @note TODO.調査中(2)
  * @brief スライスが再分割されたときにビットストリームを切断するか取得
  * @details スライスが再分割されたときにビットストリームを切断するか取得
  * @return TRUE:スライスが再分割されたときにビットストリームを切断する　FALSE:スライスが再分割されたときにビットストリームを切断しない
  */
  virtual bool getSliceReorderingConstraintFlag() const = 0;
  
  /**
  * @note TODO.調査中(2)
  * @brief スライスが再分割されたときにビットストリームを切断するか設定
  * @details スライスが再分割されたときにビットストリームを切断するか設定
  * @param[in] flag: TRUE:スライスが再分割されたときにビットストリームを切断する　FALSE:スライスが再分割されたときにビットストリームを切断しない
  */
  virtual void setSliceReorderingConstraintFlag(bool flag) = 0;

  // bool unique_point_positions_constraint_flag;
  /**
  * @brief 再構築フレームに重複点がないか取得
  * @details 再構築フレームに重複点がないか取得
  * @return TRUE:再構築フレームに重複点がない　FALSE:再構築フレームに重複点がある
  */
  virtual bool getUniquePointPositionsConstraintFlag() const = 0;

  /**
  * @brief 再構築フレームに重複点がないか設定
  * @details 再構築フレームに重複点がないか設定
  * @param[in] flag: TRUE:再構築フレームに重複点がない　FALSE:再構築フレームに重複点がある
  */
  virtual void setUniquePointPositionsConstraintFlag(bool flag) = 0;

protected:
  ProfileCompatibility() = default;
  virtual ~ProfileCompatibility() = default;
  explicit ProfileCompatibility(ProfileCompatibility&&) noexcept = default;

private:
  ProfileCompatibility(const ProfileCompatibility&) = delete;
  ProfileCompatibility& operator=(const ProfileCompatibility&) = delete;
  ProfileCompatibility& operator=(ProfileCompatibility&&) noexcept = delete;
};

//============================================================================

/**
 * @brief スケール単位
 * @details スケール単位を定義
 */
enum class ScaleUnit : bool
{
  //! 無次元
  kDimensionless = 0,
  //! メートル
  kMetre = 1,
};

//============================================================================
/**
* @brief AttributeDescriptionVector定義
* @details AttributeDescroptionクラスのベクタ列としてAttributeDescriptionVectorを定義
*/
typedef ObjectVectorAdapter<AttributeDescription> AttributeDescriptionVector;

/**
* @brief シーケンスパラメータセットクラス
* @details 対象となる点群領域に関する情報を保持するクラス
*/
class GPCCLIB_INTERFACE_SPEC SequenceParameterSet {
public:

  /**
  * @brief シーケンスパラメータセットのインスタンス生成
  * @details シーケンスパラメータセットのインスタンスを生成する
  * @return SequenceParameterSet* 生成したSequenceParameterSetのインスタンスのポインタ
  */
  static SequenceParameterSet* create();

  /**
  * @brief インスタンス削除
  * @details シーケンスパラメータセットのインスタンスを削除する
  */
  virtual void release() = 0;

  // int sps_seq_parameter_set_id;
  /**
  * @brief 該当シーケンスパラメータセットのIDを取得
  * @details 該当シーケンスパラメータセットのIDを取得
  * @return int: 該当シーケンスパラメータセットのID
  */
  virtual int getSpsSeqParameterSetId() const = 0;

  /**
  * @brief 該当シーケンスパラメータセットのIDを設定
  * @details 該当シーケンスパラメータセットのIDを設定
  * @param[in] id: 該当シーケンスパラメータセットのID
  */
  virtual void setSpsSeqParameterSetId(int id) = 0;

  // ProfileCompatibility profile;
  /**
  * @note TODO.調査中(2)
  * @brief プロファイル適合性を取得
  * @details プロファイル適合性を取得。プロファイル内容は以下
  * 　・メインプロファイルと適合しているか
  * 　・スライスが再分割されたときにビットストリームを切断するか
  * 　・再構築フレームに重複点がないか
  * @return ProfileCompatibility&: 該当シーケンスパラメータのプロファイル
  */
  virtual const ProfileCompatibility& getProfile() const = 0;

  /**
  * @note TODO.調査中(2)
  * @brief プロファイル適合性を取得
  * @details プロファイル適合性を取得。プロファイル内容は以下
  * 　・メインプロファイルと適合しているか
  * 　・スライスが再分割されたときにビットストリームを切断するか
  * 　・再構築フレームに重複点がないか
  * @return ProfileCompatibility&: 該当シーケンスパラメータのプロファイル
  */
  virtual ProfileCompatibility& getProfile() = 0;

  // int level;
  /**
  * @note TODO.調査中(2)
  * @brief 分割レベルを取得
  * @details 分割レベルを取得
  * @return int: 分割レベル
  */
  virtual int getLevel() const = 0;
  
  /**
  * @note TODO.調査中(2)
  * @brief 分割レベルを取得
  * @details 分割レベルを取得
  * @return int: 分割レベル
  */
  virtual void setLevel(int level) = 0;

  // int sps_bounding_box_offset_bits;
  /**
  * @brief バウンディングボックスの原点のコード化に使用されるビット数を取得
  * @details 現在のバウンディングボックスの原点のコード化に使用されるビット数を取得
  * @return int: バウンディングボックスの原点のコード化に使用されるビット数
  */
  virtual int getSpsBoundingBoxOffsetBits() const = 0;

  /**
  * @brief バウンディングボックスの原点のコード化に使用されるビット数を設定
  * @details 現在のバウンディングボックスの原点のコード化に使用されるビット数を設定
  * @param[in] bits: バウンディングボックスの原点のコード化に使用されるビット数
  */
  virtual void setSpsBoundingBoxOffsetBits(int bits) = 0;

  // Vec3<int> seqBoundingBoxOrigin;
  /**
  * @brief バウンディングボックスの原点を取得
  * @details 現在のバウンディングボックスの原点を取得
  * @return point_t&: バウンディングボックスの原点(Vec3<int32_t>)
  */
  virtual const point_t& getSeqBoundingBoxOrigin() const = 0;

  /**
  * @brief バウンディングボックスの原点を取得
  * @details 現在のバウンディングボックスの原点を取得
  * @return point_t&: バウンディングボックスの原点(Vec3<int32_t>)
  */
  virtual point_t& getSeqBoundingBoxOrigin() = 0;

  /**
  * @brief バウンディングボックスの原点を設定
  * @details 現在のバウンディングボックスの原点を設定
  * @param[in] point_t&: バウンディングボックスの原点(Vec3<int32_t>)
  */
  virtual void
  setSeqBoundingBoxOrigin(const point_t& seqBoundingBoxOrigin) = 0;

  // int sps_bounding_box_size_bits;
  /**
  * @brief バウンディングボックスのサイズのコード化に使用されるビット数を取得
  * @details 現在のバウンディングボックスのサイズのコード化に使用されるビット数を取得
  * @return int: バウンディングボックスのサイズのコード化に使用されるビット数
  */
  virtual int getSpsBoundingBoxSizeBits() const = 0;
  /**
  * @brief バウンディングボックスのサイズのコード化に使用されるビット数を設定
  * @details 現在のバウンディングボックスのサイズのコード化に使用されるビット数を設定
  * @param[in] bits: バウンディングボックスのサイズのコード化に使用されるビット数
  */
  virtual void setSpsBoundingBoxSizeBits(int bits) = 0;

  // Vec3<int> seqBoundingBoxSize;
  /**
  * @brief バウンディングボックスのサイズを取得
  * @details 現在のバウンディングボックスのサイズを取得
  * @return point_t&: バウンディングボックスのサイズ(Vec3<int32_t>)
  */
  virtual const point_t& getSeqBoundingBoxSize() const = 0;

  /**
  * @brief バウンディングボックスのサイズを取得
  * @details 現在のバウンディングボックスのサイズを取得
  * @return point_t&: バウンディングボックスのサイズ(Vec3<int32_t>)
  */
  virtual point_t& getSeqBoundingBoxSize() = 0;

  /**
  * @brief バウンディングボックスのサイズを設定
  * @details 現在のバウンディングボックスのサイズを設定
  * @param point_t& size: バウンディングボックスのサイズ(Vec3<int32_t>)
  */
  virtual void setSeqBoundingBoxSize(const point_t& size) = 0;

  // Rational seqGeomScale;
  /**
  * @brief 外部座標系と現バウンディングボックスの座標系の比率を取得
  * @details 外部座標系の1単位に対する現在のバウンディングボックスの座標系の1単位の比率
  * @return Rational&: 外部座標系の1単位に対する現在のバウンディングボックスの座標系の1単位の比率
  * 比率をRationalメンバによってint型分子/分母、float,double形式などで取得可能
  */
  virtual const Rational& getSeqGeomScale() const = 0;

  /**
  * @brief 外部座標系と現バウンディングボックスの座標系の比率を取得
  * @details 外部座標系の1単位に対する現在のバウンディングボックスの座標系の1単位の比率
  * @return Rational&: 外部座標系の1単位に対する現在のバウンディングボックスの座標系の1単位の比率
  * 比率をRationalメンバによってint型分子/分母、float,double形式などで取得可能
  */
  virtual Rational& getSeqGeomScale() = 0;

  // ScaleUnit seq_geom_scale_unit_flag;
  /**
  * @brief seqGeomScaleの単位を取得
  * @details seqGeomScaleの単位を取得
  * @return ScaleUnit: 0:無次元、1:メートル
  */
  virtual ScaleUnit getSeqGeomScaleUnit() const = 0;

  /**
  * @brief seqGeomScaleの単位を設定
  * @details seqGeomScaleの単位を設定
  * @param[in] ScaleUnit: 0:無次元、1:メートル
  */
  virtual void setSeqGeomScaleUnit(ScaleUnit scaleUnit) = 0;

  // TODO: GPCCLIB_INTERFACE_SPEC ???
  /**
  * @brief スケール比率を有理数として表すクラス
  * @details スケール比率を有理数の因数(int型)として表すクラス
  *         (2^denominatorLog2 + numeratorModDenominator) * 2^numeratorMulLog2
  * 比率 =  ------------------------------------------------------------------
  *                       2^denominatorLog2
  */
  class GlobalScale {
  public:
    //int numeratorMulLog2 = 0;
    /**
    * @brief NumeratorMulLog2を取得
    * @details NumeratorMulLog2を取得。GlobalScaleクラスの説明を参照
    * @return int: NumeratorMulLog2
    */
    virtual int getNumeratorMulLog2() const = 0;
    /**
    * @brief NumeratorMulLog2を設定
    * @details NumeratorMulLog2を設定。GlobalScaleクラスの説明を参照
    * @param[in] val: NumeratorMulLog2
    */
    virtual void setNumeratorMulLog2(int val) = 0;

    // int numeratorModDenominator = 0;
    /**
    * @brief numeratorModDenominatorを取得
    * @details numeratorModDenominatorを取得。GlobalScaleクラスの説明を参照
    * @return int: numeratorModDenominator
    */
    virtual int getNumeratorModDenominator() const = 0;

    /**
    * @brief numeratorModDenominatorを設定
    * @details numeratorModDenominatorを設定。GlobalScaleクラスの説明を参照
    * @param[in] val: numeratorModDenominator
    */
    virtual void setNumeratorModDenominator(int val) = 0;

    // int denominatorLog2 = 0;
    /**
    * @brief denominatorLog2を取得
    * @details denominatorLog2を取得。GlobalScaleクラスの説明を参照
    * @return int: denominatorLog2
    */
    virtual int getDenominatorLog2() const = 0;

    /**
    * @brief denominatorLog2を設定
    * @details denominatorLog2を設定。GlobalScaleクラスの説明を参照
    * @param[in] val: denominatorLog2
    */
    virtual void setDenominatorLog2(int val) = 0;

  protected:
    GlobalScale() = default;
    virtual ~GlobalScale() = default;
    explicit GlobalScale(GlobalScale&&) noexcept = default;

  private:
    GlobalScale(const GlobalScale&) = delete;
    GlobalScale& operator=(const GlobalScale&) = delete;
    GlobalScale& operator=(GlobalScale&&) noexcept = delete;
  };

  // GlobalScale globalScale;
  /**
  * @brief GlobalScaleオブジェクトを取得
  * @details GlobalScaleオブジェクトを取得。GlobalScaleクラスの説明を参照
  * @return GlobalScale& :GlobalScaleオブジェクト
  */
  virtual const GlobalScale& getGlobalScale() const = 0;

  /**
  * @brief GlobalScaleオブジェクトを取得
  * @details GlobalScaleオブジェクトを取得。GlobalScaleクラスの説明を参照
  * @return GlobalScale& :GlobalScaleオブジェクト
  */
  virtual GlobalScale& getGlobalScale() = 0;

  // std::vector<AttributeDescription> attributeSets;
  /**
  * @brief attributeSetsオブジェクトを
  * @details attributeSetsオブジェクトを取得。AttributeDescriptionのベクタを返す
  * AttributeDescriptionには以下が格納される
  * 　NumDimensionMinus1:アトリビュートの格納位置を示す　0:反射係数　2:RGB
  * 　AttributeInstanceID:インスタンスID
  * 　　注:　このインスタンスIDはデコード処理で使用される属性ID/attrIdではありません。 
  * 　　　　　インスタンスIDはデコードされた出力において同じラベルを持つ複数の属性を区別するために
  * 　　　　　使用されます。
  * 　BitDepth:bitdepth of the attribute data
  *   AttributeLable:アトリビュートラベル。KnownAttributeLabel列挙参照
  *   AttributeParamters:アトリビュートパラメタ。AttributeParamterクラス参照
  * @return AttributeDescriptionVector& :attributeSetsオブジェクト
  */
  virtual const AttributeDescriptionVector& getAttributeSets() const = 0;

  /**
  * @brief attributeSetsオブジェクトを
  * @details attributeSetsオブジェクトを取得。AttributeDescriptionのベクタを返す
  * @return AttributeDescriptionVector& :attributeSetsオブジェクト
  */
  virtual AttributeDescriptionVector& getAttributeSets() = 0;

  // int frame_ctr_bits;
  /**
  * @note TODO.調査中(2)
  * @brief フレームコントロールビットを取得
  * @details タイル分割時のフレーム管理番号のビット数であるフレームコントロールビットを取得
  * @return int: フレームコントロールビットを取得
  */
  virtual int getFrameCtrBits() const = 0;

   /**
  * @note TODO.調査中(2)
  * @brief フレームコントロールビットを設定
  * @details タイル分割時のフレーム管理番号のビット数であるフレームコントロールビットを設定
  * @param[in] int bits: フレームコントロールビットを設定
  */
  virtual void setFrameCtrBits(int bits) = 0;

  // int slice_tag_bits;
  /**
  * @brief スライスのタグのビット数を取得
  * @details スライスのタグのビット数を取得
  * @return int: スライスのタグのビット数
  */
  virtual int getSliceTagBits() const = 0;

  /**
  * @brief スライスのタグのビット数を設定
  * @details スライスのタグのビット数を設定
  * @return int bits: スライスのタグのビット数
  */
  virtual void setSliceTagBits(int bits) = 0;

  // AxisOrder geometry_axis_order;
  /**
  * @brief 座標順番を取得
  * @details 座標順番を取得。AxisOrderクラス参照
  * @return AxisOrder: 座標順番
  */
  virtual AxisOrder getGeometryAxisOrder() const = 0;

  /**
  * @brief 座標順番を設定
  * @details 座標順番を設定。AxisOrderクラス参照
  * @param[in] AxisOrder axisOrder: 座標順番
  */
  virtual void setGeometryAxisOrder(AxisOrder axisOrder) = 0;

  // bool cabac_bypass_stream_enabled_flag;
  /**
  * @note TODO.調査中(2)
  * @brief エントロピー符号化法を取得
  * @details 等確率（バイパス）データに使用されるエントロピー符号化法にCABACが有効かを取得
  *   | 値 |　記述                           |
  *   |:-----:| --------------------------------------|
  *   | 0     | バイパスデータはCABACでコーディング        |
  *   | 1     | バイパスデータはバイパスビットストリームにコーディングされる |
  * @return TRUE: エントロピー符号化にCABACが使用される
  * @return FALSE: エントロピー符号はバイパスサブストリームに格納される
  */
  virtual bool isCabacBypassStreamEnabled() const = 0;

  /**
  * @note TODO.調査中(2)
  * @brief エントロピー符号化法を設定
  * @details 等確率（バイパス）データに使用されるエントロピー符号化法にCABACが有効かを設定
  * @param[in] bool enabled TRUE: エントロピー符号化にCABACが有効　
  * @param[in] bool enabled FALSE: エントロピー符号はバイパスサブストリームに格納される
  */
  virtual void setCabacBypassStreamEnabled(bool enabled) = 0;

  // bool entropy_continuation_enabled_flag;
  /**
  * @brief スライス間のエントロピー符号化が有効か取得
  * @details 同一フレーム内のスライス間のエントロピー符号化状態（コンテキスト値）の伝搬が有効か取得
  * @return TRUE: 有効 FALSE: 無効
  */
  virtual bool isEntropyContinuationEnabled() const = 0;

  /**
  * @brief スライス間のエントロピー符号化の有効無効設定
  * @details 同一フレーム内のスライス間のエントロピー符号化状態（コンテキスト値）の伝搬が有効/無効を設定
  * @param[in] bool enabled TRUE: 有効 FALSE: 無効
  */
  virtual void setEntropyContinuationEnabled(bool enabled) = 0;

protected:
  SequenceParameterSet() = default;
  virtual ~SequenceParameterSet() = default;
  explicit SequenceParameterSet(SequenceParameterSet&&) noexcept = default;

private:
  SequenceParameterSet(const SequenceParameterSet&) = delete;
  SequenceParameterSet& operator=(const SequenceParameterSet&) = delete;
  SequenceParameterSet& operator=(SequenceParameterSet&&) noexcept = delete;
};

//============================================================================

/**
* @brief ジオメトリパラメータセットクラス
* @details 幾何情報に関するパラメータを保持するクラス
*/
class GPCCLIB_INTERFACE_SPEC GeometryParameterSet {
public:
  /**
  * @brief ジオメトリパラメータセットのインスタンス生成
  * @details ジオメトリパラメータセットのインスタンスを生成する
  * @return GeometryParameterSet* 生成したGeometryParameterSetのインスタンスのポインタ
  */
  static GeometryParameterSet* create();

  /**
  * @brief インスタンス削除
  * @details ジオメトリパラメータセットのインスタンスを削除する
  */
  virtual void release() = 0;

  // int gps_geom_parameter_set_id;
  /**
  * @brief ジオメトリパラメータセット中のジオメトリパラメータセットIDを取得
  * @details ジオメトリパラメータセット中のジオメトリパラメータセットIDを取得
  * @return int: ジオメトリパラメータセット中のジオメトリパラメータセットID(gps_geom_parameter_set_id)
  */
  virtual int getGpsGeomParameterSetId() const = 0;

  /**
  * @brief ジオメトリパラメータセット中のジオメトリパラメータセットIDを設定
  * @details ジオメトリパラメータセット中のジオメトリパラメータセットIDを設定
  * @param[in] int id: ジオメトリパラメータセット中のジオメトリパラメータセットID
  */
  virtual void setGpsGeomParameterSetId(int id) = 0;

  // int gps_seq_parameter_set_id;
  /**
  * @brief ジオメトリパラメータセット中のシーケンスパラメータセットIDを取得
  * @details ジオメトリパラメータセット中のシーケンスパラメータセットIDを取得
  * @return int: ジオメトリパラメータセット中のシーケンスパラメータセットID(gps_seq_parameter_set_id)
  */
  virtual int getGpsSeqParameterSetId() const = 0;

  /**
  * @brief ジオメトリパラメータセット中のシーケンスパラメータセットIDを設定
  * @details ジオメトリパラメータセット中のシーケンスパラメータセットIDを設定
  * @param[in] int id: ジオメトリパラメータセット中のシーケンスパラメータセットID
  */
  virtual void setGpsSeqParameterSetId(int id) = 0;

  // bool geom_box_log2_scale_present_flag;
  /**
  * @note TODO.調査中(2)
  * @brief gps_geom_box_log2_scale と geom_box_log2_scale の存在を示す
  * @details gps_geom_box_log2_scale と geom_box_log2_scale の存在を示す
  * @return TRUE: gps_geom_box_log2_scale と geom_box_log2_scale が存在
  * @return FALSE:gps_geom_box_log2_scale と geom_box_log2_scale が存在しない
  */
  virtual bool getGeomBoxLog2ScalePresentFlag() const = 0;

  /**
  * @note TODO.調査中(2)
  * @brief gps_geom_box_log2_scale と geom_box_log2_scale の存在を設定
  * @details gps_geom_box_log2_scale と geom_box_log2_scale の存在を設定
  * @param[in] bool flag TRUE: gps_geom_box_log2_scale と geom_box_log2_scale が存在する
  * @param[in] bool flag FALSE:gps_geom_box_log2_scale と geom_box_log2_scale が存在しない
  */
  virtual void setGeomBoxLog2ScalePresentFlag(bool flag) = 0;

  // int gps_geom_box_log2_scale;
  /**
  * @note TODO.調査中(2)
  * @brief スライスごとのジオメトリボックスの原点に対するデフォルトのスケーリング係数を取得
  * @details スライスごとのジオメトリボックスの原点に対するデフォルトのスケーリング係数を取得
  * @return int: スライスごとのジオメトリボックスの原点に対するデフォルトのスケーリング係数
  */
  virtual int getGpsGeomBoxLog2Scale() const = 0;

  /**
  * @note TODO.調査中(2)
  * @brief スライスごとのジオメトリボックスの原点に対するデフォルトのスケーリング係数を設定
  * @details スライスごとのジオメトリボックスの原点に対するデフォルトのスケーリング係数を設定
  * @param[in] int val: スライスごとのジオメトリボックスの原点に対するデフォルトのスケーリング係数
  */
  virtual void setGpsGeomBoxLog2Scale(int val) = 0;

  // bool predgeom_enabled_flag;
  /**
  * @brief 予測ジオメトリーコーディングの有効/無効を取得
  * @details 予測ジオメトリーコーディングとオクトリージオメトリーコーディングの選択を取得
  * @return TRUE: 予測ジオメトリーコーディングが有効
  * @return FALSE:オクトリージオメトリーコーディングが有効
  */
  virtual bool isPredgeomEnabled() const = 0;
  /**
  * @brief 予測ジオメトリーコーディングの有効/無効を設定
  * @details 予測ジオメトリーコーディングとオクトリージオメトリーコーディングの選択を設定
  * @return bool enabled: TRUE: 予測ジオメトリーコーディングが有効
  * @return bool enabled: FALSE:オクトリージオメトリーコーディングが有効
  */
  virtual void setPredgeomEnabled(bool enabled) = 0;

  // bool geom_unique_points_flag;
  /**
  * @note TODO.調査中(2)
  * @brief 同じ空間位置に点は1つかどうかを取得
  * @details 同じ空間位置にある複数の点（関連する属性を持つ）を表現する機能（リカラーリング）を制御する
  * @return TRUE: 同じ空間位置に点は1つ
  * @return FALSE:同じ空間位置に点は複数
  */
  virtual bool getGeomUniquePointsFlag() const = 0;

  /**
  * @note TODO.調査中(2)
  * @brief 同じ空間位置に点は1つかどうかを設定
  * @details 同じ空間位置にある複数の点（関連する属性を持つ）を表現する機能（リカラーリング）を制御する
  * @param[in] bool flag: TRUE: 同じ空間位置に点は1つ
  * @param[in] bool flag: FALSE:同じ空間位置に点は複数
  */
  virtual void setGeomUniquePointsFlag(bool flag) = 0;

  // int neighbour_avail_boundary_log2_minus1;
   /**
  * @note TODO.調査中(2)
  * @brief 占有検索のための近隣利用可能ボリュームのサイズを取得
  * @details オクトリーノードが占有状況コンテキスト化と占有内予測で使用可能とみなされるボリュームを取得 
  * 2未満は隣接ノードの使用をオクトリーの直接のsiblingに制限します。
　* ソフトウェアでは占有率予測が有効または無効の場合、それぞれ最大値8または9をサポートしています。
  * @return int: 占有検索のための近隣利用可能ボリュームのサイズ(サイズのビット数－1)
  */
  virtual int getNeighbourAvailBoundaryLog2Minus1() const = 0;

  /**
  * @note TODO.調査中(2)
  * @brief 占有検索のための近隣利用可能ボリュームのサイズを設定
  * @details オクトリーノードが占有状況コンテキスト化と占有内予測で使用可能とみなされるボリュームを設定 
  * 2未満は隣接ノードの使用をオクトリーの直接のsiblingに制限します。
　* ソフトウェアでは占有率予測が有効または無効の場合、それぞれ最大値8または9をサポートしています。
  * @param[in] int val: 占有検索のための近隣利用可能ボリュームのサイズ(サイズのビット数－1)
  */
  virtual void setNeighbourAvailBoundaryLog2Minus1(int val) = 0;

  // int inferred_direct_coding_mode;
  /**
  * @note TODO.調査中(2)
  * @brief 孤立点のジオメトリ・オクトリーの早期終了を制御する値を取得
  * @details 孤立点のコードにおけるジオメトリ・オクトリーの早期終了を制御する値を取得
  *  | 値｜修飾ノード基準の範囲
  *  |:-:| -------------------|
  *  | 0 ｜ 無効
  *  | 1 ｜ 完全に孤立した親子
  *  | 2 ｜ 部分的に孤立した親
  *  | 3 ｜ 制約なし
  * @return int: 孤立点のジオメトリ・オクトリーの早期終了を制御する値
  */
  virtual int getInferredDirectCodingMode() const = 0;
  
  /**
  * @note TODO.調査中(2)
  * @brief 孤立点のジオメトリ・オクトリーの早期終了を制御する値を設定
  * @details 孤立点のコードにおけるジオメトリ・オクトリーの早期終了を制御する値を設定
  *  | 値｜修飾ノード基準の範囲
  *  |:-:| -------------------|
  *  | 0 ｜ 無効
  *  | 1 ｜ 完全に孤立した親子
  *  | 2 ｜ 部分的に孤立した親
  *  | 3 ｜ 制約なし
  * @return int val: 孤立点のジオメトリ・オクトリーの早期終了を制御する値
  */
  virtual void setInferredDirectCodingMode(int val) = 0;

  // bool joint_2pt_idcm_enabled_flag;
  /**
  * @note TODO.調査中(2)
  * @brief 2つの孤立点を含むダイレクトコーディングの有効/無効を取得
  * @details 2つの孤立点を含むダイレクトコーディングされたノードの点位置を
  * コーディングする方法を制御する情報を取得。
  * 有効にすると暗黙的な点順序が使用されコーディング効率が向上します。
  * @return bool: TRUE:2つの孤立点を含むダイレクトコーディングが有効
  * @return bool: FALSE:2つの孤立点を含むダイレクトコーディングが無効
  */
  virtual bool isJoint2ptIdcmEnabled() const = 0;
  
  /**
  * @note TODO.調査中(2)
  * @brief 2つの孤立点を含むダイレクトコーディングの有効/無効を設定
  * @details 2つの孤立点を含むダイレクトコーディングされたノードの点位置を
  * コーディングする方法を制御する情報を設定
  * 有効にすると暗黙的な点順序が使用されコーディング効率が向上します。
  * @param[in] bool enabled: TRUE:2つの孤立点を含むダイレクトコーディングが有効
  * @param[in] bool enabled: FALSE:2つの孤立点を含むダイレクトコーディングが無効
  */
  virtual void setJoint2ptIdcmEnabled(bool enabled) = 0;

  // bool bitwise_occupancy_coding_flag;
  /**
  * @note TODO.調査中(2)
  * @brief オクツリージオメトリ符号化方法を取得
  * @details オクツリージオメトリ符号化では占有データを符号化するためにバイト単位とビット単位
  * がありビット単位が有効であることを示す。
  * @return bool: TRUE:ビット単位が有効
  * @return bool: FALSE:ビット単位が無効（バイト単位が有効）
  */
  virtual bool getBitwiseOccupancyCodingFlag() const = 0;

  /**
  * @note TODO.調査中(2)
  * @brief オクツリージオメトリ符号化方法を設定
  * @details オクツリージオメトリ符号化では占有データを符号化するためにバイト単位とビット単位
  * がありビット単位が有効であることを示す。
  * @param[in] bool flag: TRUE:ビット単位が有効
  * @param[in] bool flag: FALSE:ビット単位が無効（バイト単位が有効）
  */
  virtual void setBitwiseOccupancyCodingFlag(bool flag) = 0;

  // bool adjacent_child_contextualization_enabled_flag;
  /**
  * @note TODO.調査中(2)
  * @brief 隣接ノードの子の状態による占有ビットのコンテキスト化の有効/無効を取得
  * @details 本値は隣接するノードの隣接する子の状態に応じて占有ビットのコンテキスト化を制御する
  * @return bool: TRUE:隣接するノードの隣接する子の状態に応じて占有ビットのコンテキスト化が有効
  * @return bool: FALSE:隣接するノードの隣接する子の状態に応じて占有ビットのコンテキスト化が無効
  */
  virtual bool isAdjacentChildContextualizationEnabled() const = 0;

  /**
  * @note TODO.調査中(2)
  * @brief 隣接ノードの子の状態による占有ビットのコンテキスト化の有効/無効を設定
  * @details 隣接するノードの隣接する子の状態に応じて占有ビットのコンテキスト化を制御するフラグを設定
  * @return bool enabled: TRUE:隣接するノードの隣接する子の状態に応じて占有ビットのコンテキスト化が有効
  * @return bool enabled: FALSE:隣接するノードの隣接する子の状態に応じて占有ビットのコンテキスト化が無効
  */
  virtual void setAdjacentChildContextualizationEnabled(bool enabled) = 0;

  // int intra_pred_max_node_size_log2;
  /**
  * @note TODO.調査中(2)
  * @brief 内部占有予測が有効なノードの最大サイズ取得
  * @details 本値によって内部占有予測はオクツリーノードの近隣ノードを使用して占有率を予測する。
  * この予測モードは設定されたサイズ以下のオクトリーノードに対して有効。
  * 値が0の場合、内部占有率内予測は無効
  * @return int: 内部占有予測が有効なノードの最大サイズ
  */
  virtual int getIntraPredMaxNodeSizeLog2() const = 0;

  /**
  * @note TODO.調査中(2)
  * @brief 内部占有予測が有効なノードの最大サイズ設定
  * @details 本値によって内部占有予測はオクツリーノードの近隣ノードを使用して占有率を予測する。
  * この予測モードは設定されたサイズ以下のオクトリーノードに対して有効。
  * 値が0の場合、内部占有率内予測は無効
  * @param[in] int val: 内部占有予測が有効なノードの最大サイズ
  */
  virtual void setIntraPredMaxNodeSizeLog2(int val) = 0;

  // bool trisoup_enabled_flag;
  /**
  * @brief Trisoupジオメトリコーディングの有効/無効を取得
  * @details Trisoupジオメトリコーディングの有効/無効を取得
  * @return bool: TRUE:有効　FALSE:無効 
  */
  virtual bool isTrisoupEnabled() const = 0;

  /**
  * @brief Trisoupジオメトリコーディングの有効/無効を設定
  * @details Trisoupジオメトリコーディングの有効/無効を設定
  * @param[in] bool enabled: TRUE:有効　FALSE:無効 
  */
  virtual void setTrisoupEnabled(bool enabled) = 0;

  // int trisoup_sampling_value;
  /**
  * @brief Trisoupサンプリング値取得
  * @details Trisoupコーディングの三角形ボクセル化処理によって生成される点の数を制御します。
  * 値が大きいほど三角形ごとに生成される点の数が減少します。
  *  | 値    |
  *  |:-----:| -------------------------------|
  *  | 0     |自動（デフォルト
  *  | 1     |サブサンプリングなし
  *  | N >=1 |ポイント・サンプリング周期
  * 
  * 自動モードは最小サンプリング値をsliceMaxPointsによって設定されたスライス制限を超えない値
  * として設定します。
  * @return int: Trisoupサンプリング値
  */
  virtual int getTrisoupSamplingValue() const = 0;

  /**
  * @brief Trisoupサンプリング値設定
  * @details Trisoupコーディングの三角形ボクセル化処理によって生成される点の数を制御します。
  * 値が大きいほど三角形ごとに生成される点の数が減少します。
  *  | 値    |
  *  |:-----:| -------------------------------|
  *  | 0     |自動（デフォルト
  *  | 1     |サブサンプリングなし
  *  | N >=1 |ポイント・サンプリング周期
  * 
  * 自動モードは最小サンプリング値をsliceMaxPointsによって設定されたスライス制限を超えない値
  * として設定します。
  * @return int val: Trisoupサンプリング値
  */
  virtual void setTrisoupSamplingValue(int val) = 0;

  // bool geom_scaling_enabled_flag;
  /**
  * @note TODO.調査中(2)
  * @brief オクトリーコーディング内でのジオメトリスケーリング機能の有効/無効を取得
  * @details オクトリーコーディング内でのジオメトリスケーリング機能の有効/無効を取得
  * @return bool: TRUE:ジオメトリスケーリング機能有効　FALSE:ジオメトリスケーリング機能無効 
  */
  virtual bool isGeomScalingEnabled() const = 0;
  
  /**
  * @note TODO.調査中(2)
  * @brief オクトリーコーディング内でのジオメトリスケーリング機能の有効/無効を設定
  * @details オクトリーコーディング内でのジオメトリスケーリング機能の有効/無効を設定
  * @param[in] bool enabled: TRUE:ジオメトリスケーリング機能有効　FALSE:ジオメトリスケーリング機能無効 
  */
  virtual void setGeomScalingEnabled(bool enabled) = 0;

  // int geom_qp_multiplier_log2;
  /**
  * @note TODO.調査中(2)　確認Must！！
  * @brief ジオメトリQPをシフトする係数を取得
  * @details ステップサイズ2倍毎のQP値の数を制限することで量子化ステップサイズの粒度を制御します。 
  * ステップサイズ2倍毎に2^n個のQPがあります。
  * 
  * @return int: ジオメトリQPをシフトする係数
  */
  virtual int getGeomQpMultiplierLog2() const = 0;

  /**
  * @note TODO.調査中(2)　確認Must！！
  * @brief ジオメトリQPをシフトする係数を設定
  * @details ステップサイズ2倍毎のQP値の数を制限することで量子化ステップサイズの粒度を制御します。 
  * ステップサイズ2倍毎に2^n個のQPがあります。
  * 
  * @return int val: ジオメトリQPをシフトする係数
  */
  virtual void setGeomQpMultiplierLog2(int val) = 0;

  // int geom_base_qp;
  /**
  * @note TODO.調査中(2)　確認Must！！
  * @brief ジオメトリスケーリング用の初期qp値取得
  * @details ジオメトリスケーリング用の初期qp値（qpマルチプライヤでスケーリングされる）
  * @return int: ジオメトリスケーリング用の初期qp値
  */
  virtual int getGeomBaseQp() const = 0;
  
  /**
  * @note TODO.調査中(2)　確認Must！！
  * @brief ジオメトリスケーリング用の初期qp値設定
  * @details ジオメトリスケーリング用の初期qp値（qpマルチプライヤでスケーリングされる）
  * @param[in] int qp: ジオメトリスケーリング用の初期qp値
  */
  virtual void setGeomBaseQp(int qp) = 0;

  // int geom_idcm_qp_offset;
  /**
  * @note TODO.調査中(2)　確認Must！！
  * @brief IDCMポジション定量化パラメータ取得
  * @details positionQuantisationOctreeDepth`に到達する前に直接符号化された（IDCM）点の位置を
  * 定量化するために使用されるパラメータ
  * qp乗数でスケーリングされたidcmノードの初期qpのオフセット値
  * @return int: IDCMポジション定量化パラメータ
  */
  virtual int getGeomIdcmQpOffset() const = 0;
  /**
  * @note TODO.調査中(2)　確認Must！！
  * @brief IDCMポジション定量化パラメータ設定
  * @details positionQuantisationOctreeDepth`に到達する前に直接符号化された（IDCM）点の位置を
  * 定量化するために使用されるパラメータ
  * qp乗数でスケーリングされたidcmノードの初期qpのオフセット値
  * @return int val: IDCMポジション定量化パラメータ
  */
  virtual void setGeomIdcmQpOffset(int val) = 0;

  // bool qtbt_enabled_flag;
  /**
  * @brief 非立方体のジオメトリーコーディングの有効/無効を取得
  * @details 非立方体のジオメトリツリーコーディングを有効にすると、
  * ジオメトリツリーは直方体のバウンディング ボックスを持つことができます。
  * 特定の深さにおける内部ツリーノードのパーティショニングはエンコーダによって非正規に決定され
  * オクトリー、クワッドツリー、またはバイナリパーティションのいずれかになります。
  * @return bool: TRUE:非立方体のジオメトリーコーディング有効　FALSE:非立方体のジオメトリーコーディングの無効 
  */
  virtual bool isQtbtEnabled() const = 0;

  /**
  * @brief 非立方体のジオメトリーコーディングの有効/無効を設定
  * @details 非立方体のジオメトリツリーコーディングを有効にすると、
  * ジオメトリツリーは直方体のバウンディング ボックスを持つことができます。
  * 特定の深さにおける内部ツリーノードのパーティショニングはエンコーダによって非正規に決定され
  * オクトリー、クワッドツリー、またはバイナリパーティションのいずれかになります。
  * @param[in] bool enabled: TRUE:非立方体のジオメトリーコーディング有効　FALSE:非立方体のジオメトリーコーディングの無効 
  */
  virtual void setQtbtEnabled(bool enabled) = 0;

  // bool geom_planar_mode_enabled_flag;
  // int geom_planar_threshold0;
  // int geom_planar_threshold1;
  // int geom_planar_threshold2;
  // int geom_idcm_rate_minus1;
  /**
  * @brief Planarモードの有効/無効を取得
  * @details ジオメトリの占有にPlanarモードの使用を制御します。
  * Planarモードはオクツリーの各対象ノードをより効率的にコーディングします。
  * @return bool: TRUE:Planarモード有効　FALSE:Planarモード無効 
  */
  virtual bool isGeomPlanarModeEnabled() const = 0;
  
  /**
  * @brief Planarモードの有効/無効を設定
  * @details ジオメトリの占有にPlanarモードの使用を制御します。
  * Planarモードはオクツリーの各対象ノードをより効率的にコーディングします。
  * @param[in] bool enabled: TRUE:Planarモード有効　FALSE:Planarモード無効 
  */
  virtual void setGeomPlanarModeEnabled(bool enabled) = 0;

  /**
  * @brief Planarモードの第一閾値を取得
  * @details 本値(0-127)はローカルな子ノード密度に基づいて最初の平面モードの適格性閾値を制御します。
  * @return int: Planarモードの第一閾値 
  */
  virtual int getGeomPlanarThreshold0() const = 0;

  /**
  * @brief Planarモードの第一閾値を設定
  * @details 本値(0-127)はローカルな子ノード密度に基づいて最初の平面モードの適格性閾値を制御します。
  * @param[in] int val: Planarモードの第一閾値 
  */
  virtual void setGeomPlanarThreshold0(int val) = 0;

  /**
  * @brief Planarモードの第二閾値を取得
  * @details 本値(0-127)はローカルな子ノード密度に基づいて2番目の平面モードの適格性閾値を制御します。
  * @return int: Planarモードの第二閾値 
  */
  virtual int getGeomPlanarThreshold1() const = 0;
  
  /**
  * @brief Planarモードの第二閾値を設定
  * @details 本値(0-127)はローカルな子ノード密度に基づいて2番目の平面モードの適格性閾値を制御します。
  * @param[in] int val: Planarモードの第二閾値 
  */
  virtual void setGeomPlanarThreshold1(int val) = 0;

  /**
  * @brief Planarモードの第三閾値を取得
  * @details 本値(0-127)はローカルな子ノード密度に基づいて3番目の平面モードの適格性閾値を制御します。
  * @return int: Planarモードの第三閾値 
  */
  virtual int getGeomPlanarThreshold2() const = 0;

  /**
  * @brief Planarモードの第三閾値を設定
  * @details 本値(0-127)はローカルな子ノード密度に基づいて3番目の平面モードの適格性閾値を制御します。
  * @param[in] int val: Planarモードの第三閾値 
  */
  virtual void setGeomPlanarThreshold2(int val) = 0;

  /**
  * @note TODO.調査中(2)　Must！！
  * @brief IDCMレートを取得
  * @details ジオメトリーコーディングで使用するIDCMレートを取得
  * IDCMの適合性を1/32パーセント単位で周波数を制御する。 
  * 0 :IDCMは無効 32 :IDCMは制約を受けない。
  * @return int: IDCMレートを取得
  */
  virtual int getGeomIdcmRateMinus1() const = 0;

   /**
  * @note TODO.調査中(2)　Must！！
  * @brief IDCMレートを設定
  * @details ジオメトリーコーディングで使用するIDCMレートを設定
  * IDCMの適合性を1/32パーセント単位で周波数を制御する。 
  * 0 :IDCMは無効 32 :IDCMは制約を受けない。
  * @param[in] int val: IDCMレート
  */
  virtual void setGeomIdcmRateMinus1(int val) = 0;

  // bool geom_angular_mode_enabled_flag;
  /**
  * @brief 角度モードの有効/無効を取得
  * @details 本値はジオメトリ占有における角度コーディングモードの使用を制御します。 
  * 角度コーディングモードはLiDaRで取得した点群の圧縮を改善するために、事前にLiDaRヘッドモデルを
  * 使用します。 角度モードは平面(Planar)モードが有効になっている必要があります。
  * @return bool: TRUE: 角度モード有効　FALSE: 角度モード無効
  */
  virtual bool isGeomAngularModeEnabled() const = 0;

  /**
  * @brief 角度モードの有効/無効を設定
  * @details 本値はジオメトリ占有における角度コーディングモードの使用を制御します。 
  * 角度コーディングモードはLiDaRで取得した点群の圧縮を改善するために、事前にLiDaRヘッドモデルを
  * 使用します。 角度モードは平面(Planar)モードが有効になっている必要があります。
  * @parma[in] bool enabled: TRUE: 角度モード有効　FALSE: 角度モード無効
  */
  virtual void setGeomAngularModeEnabled(bool enabled) = 0;

  // bool geom_slice_angular_origin_present_flag;
  /**
  * @brief 角度モードの原点の格納先を取得
  * @details 本値は角度モードの原点がスライスかジオメトリパラメータセットのどちらかに内蔵されているかを制御します。
  * @return bool:
  * TRUE: 角度モードの原点はgbhAngularOriginとしてスライスに格納されている
  * FALSE:角度モードの原点はgpsAngularOriginとしてジオメトリパラメータセットに格納されている
  */
  virtual bool getGeomSliceAngularOriginPresentFlag() const = 0;

  /**
  * @brief 角度モードの原点の格納先を設定
  * @details 本値角度モードの原点がスライスかジオメトリパラメータセットのどちらかに内蔵されているかを制御します。
  * @param[in] bool flag:
  * TRUE: 角度モードの原点はgbhAngularOriginとしてスライスに格納されている
  * FALSE:角度モードの原点はgpsAngularOriginとしてジオメトリパラメータセットに格納されている
  */
  virtual void setGeomSliceAngularOriginPresentFlag(bool flag) = 0;

  // (in stv axis order).
  // Vec3<int> gpsAngularOrigin;
  /**
  * @brief 角度モードの原点の格納先を取得
  * @details 本値は角度モードの原点を取得します。
  * @return point_t&: 本値は角度モードの原点
  */
  virtual const point_t& getGpsAngularOrigin() const = 0;

  /**
  * @brief 角度モードの原点の格納先を取得
  * @details 本値は角度モードの原点を取得します。
  * @return point_t&: 本値は角度モードの原点
  */
  virtual point_t& getGpsAngularOrigin() = 0;

  /**
  * @brief 角度モードの原点を設定
  * @details 本値は角度モードの原点を設定します。
  * @param[in] point_t& origin: 本値は角度モードの原点
  */
  virtual void setGpsAngularOrigin(const point_t& origin) = 0;

  // std::vector<int> angularTheta;
  // std::vector<int> angularZ;
  // std::vector<int> angularNumPhiPerTurn;
  /**
  * @brief 角度モードの仰角θを取得
  * @details 本値は角度モードで使用される各既知レーザーの仰角θ示します。
  * 各仰角はx-y平面に対するラジアン単位で表されます。
  * ゼロの角度：水平なレーザー 正の角度：水平より高い仰角　負の角度：水平より下の仰角
  * @return Int32VectorAdapter&: 角度モードの各レーザーの仰角ベクトル列
  */
  virtual const Int32VectorAdapter& getAngularTheta() const = 0;

  /**
  * @brief 角度モードの仰角θを取得
  * @details 本値は角度モードで使用される各既知レーザーの仰角θ示します。
  * 各仰角はx-y平面に対するラジアン単位で表されます。
  * ゼロの角度：水平なレーザー 正の角度：水平より高い仰角　負の角度：水平より下の仰角
  * @return Int32VectorAdapter&: 角度モードの各レーザーの仰角ベクトル列
  */
  virtual Int32VectorAdapter& getAngularTheta() = 0;

  /**
  * @brief 角度モードの垂直オフセットを取得
  * @details 本値は角度モードで使用される各既知レーザーの垂直オフセットを示します。
  * 各オフセットは入力座標系のz軸に沿って表現され、LiDARヘッド位置に対する垂直オフセットに対応します。
  * @return Int32VectorAdapter&: 角度モードで使用される各既知レーザーの垂直オフセットベクトル列
  */
  virtual const Int32VectorAdapter& getAngularZ() const = 0;
  
  /**
  * @brief 角度モードの垂直オフセットを取得
  * @details 本値は角度モードで使用される各既知レーザーの垂直オフセットを示します。
  * 各オフセットは入力座標系のz軸に沿って表現され、LiDARヘッド位置に対する垂直オフセットに対応します。
  * @return Int32VectorAdapter&: 角度モードで使用される各既知レーザーの垂直オフセットベクトル列
  */
  virtual Int32VectorAdapter& getAngularZ() = 0;

  /**
  * @brief 角度モードで各レーザーが取得できる最大サンプル数を取得
  * @details 本値は角度モードで使用される既知の各レーザーが1回転する間に取得できる
  * サンプルの最大数を示します。
  * @return Int32VectorAdapter&: 角度モードで各レーザーが取得できる最大サンプル数のベクトル列
  */
  virtual const Int32VectorAdapter& getAngularNumPhiPerTurn() const = 0;

  /**
  * @brief 角度モードで各レーザーが取得できる最大サンプル数を取得
  * @details 本値は角度モードで使用される既知の各レーザーが1回転する間に取得できる
  * サンプルの最大数を示します。
  * @return Int32VectorAdapter&: 角度モードで各レーザーが取得できる最大サンプル数のベクトル列
  */
  virtual Int32VectorAdapter& getAngularNumPhiPerTurn() = 0;
  
  /**
  * @brief 角度モードで各レーザーが取得できる最大サンプル数を取得
  * @details 本値は角度モードで使用される既知の各レーザーが1回転する間に取得できる
  * サンプルの最大数を示します。
  * @return Int32VectorAdapter&: 角度モードで各レーザーが取得できる最大サンプル数のベクトル列
  */
  virtual int getGeomAngularThetaPred(int i) const = 0;

  // bool planar_buffer_disabled_flag;
  /**
  * @brief 角度モードの平面モードバッファの無効化の有効/無効を取得
  * @details 本値は角度モードの平面モードバッファの無効化を制御します。
  * @return bool: TRUE: 角度モードの平面モードバッファは無効
  * @return bool: FALSE: 角度モードの平面モードバッファは有効
  */
  virtual bool isPlanarBufferDisabled() const = 0;

  /**
  * @brief 角度モードの平面モードバッファの無効化の有効/無効を設定
  * @details 本値は角度モードの平面モードバッファの無効化を制御します。
  * @param[in] bool disabled: TRUE: 角度モードの平面モードバッファは無効
  * @param[in] bool disabled: FALSE: 角度モードの平面モードバッファは有効
  */
  virtual void setPlanarBufferDisabled(bool disabled) = 0;

  // int geom_qp_offset_intvl_log2;
  /**
  * @brief 予測ジオメトリコーディングにおけるブロックサイズを取得
  * @details 本値は予測ジオメトリコーディングにおけるブロックサイズ（ブロックあたりのポイント数)
  * を示します。
  * @return int: 予測ジオメトリコーディングにおけるブロックあたりのポイント数
  */
  virtual int getGeomQpOffsetIntvlLog2() const = 0;

  /**
  * @brief 予測ジオメトリコーディングにおけるブロックサイズを設定
  * @details 本値は予測ジオメトリコーディングにおけるブロックサイズ（ブロックあたりのポイント数)
  * を示します。
  * @param[in] int val: 予測ジオメトリコーディングにおけるブロックあたりのポイント数
  */
  virtual void setGeomQpOffsetIntvlLog2(int val) = 0;

  // int geom_angular_azimuth_scale_log2_minus11;
  /**
  * @note TODO.調査中(3) 
  * @brief 方位角を表現するために使用される追加ビット数を取得
  * @details 本値は予測ジオメトリの方位角を表現するために使用される追加ビット数です。
  * 本値を有効にするにはangularEnabled=1が必要です。
  * @return int: 予測ジオメトリの方位角を表現するために使用される追加ビット数
  */
  virtual int getGeomAngularAzimuthScaleLog2Minus11() const = 0;
  
  /** 
  * @brief 方位角を表現するために使用される追加ビット数を設定
  * @details 本値は予測ジオメトリの方位角を表現するために使用される追加ビット数です。
  * 本値を有効にするにはangularEnabled=1が必要です。
  * @param[in] int val: 予測ジオメトリの方位角を表現するために使用される追加ビット数
  */
  virtual void setGeomAngularAzimuthScaleLog2Minus11(int val) = 0;
  // int geom_angular_azimuth_speed_minus1;
  /**
  * @note TODO.調査中(3) 
  * @brief 方位角の進行ステップサイズを取得
  * @details 本値は角度予測ジオメトリコーディング中にレーザーごとの方位角の進行を線形に
  * モデル化するために使用されるステップサイズ。
  * 本値を有効にするにはangularEnabled=1が必要です。
  * @return int: レーザーごとの方位角の進行に使用されるステップサイズ
  */
  virtual int getGeomAngularAzimuthSpeedMinus1() const = 0;

  /**
  * @brief 方位角の進行ステップサイズを設定
  * @details 本値は角度予測ジオメトリコーディング中にレーザーごとの方位角の進行を線形に
  * モデル化するために使用されるステップサイズ。
  * 本値を有効にするにはangularEnabled=1が必要です。
  * @param[in] int val: レーザーごとの方位角の進行に使用されるステップサイズ
  */
  virtual void setGeomAngularAzimuthSpeedMinus1(int val) = 0;

  // int geom_angular_radius_inv_scale_log2;
  /**
  * @note TODO.調査中(3) 
  * @brief 予測ジオメトリ符号化における半径符号化の逆縮小係数の取得
  * @details 本値は角度予測ジオメトリの半径方向距離の表現に適用される量子化程度を示します。
  * 本値を有効にするにはangularEnabled=1が必要です。
  * @return int: 予測ジオメトリ符号化における半径符号化の逆縮小係数
  */
  virtual int getGeomAngularRadiusInvScaleLog2() const = 0;
  
  /**
  * @brief 予測ジオメトリ符号化における半径符号化の逆縮小係数の設定
  * @details 本値は角度予測ジオメトリの半径方向距離の表現に適用される量子化程度を示します。
  * 本値を有効にするにはangularEnabled=1が必要です。
  * @param[in] int val: 予測ジオメトリ符号化における半径符号化の逆縮小係数
  */
  virtual void setGeomAngularRadiusInvScaleLog2(int val) = 0;

  // in each octree level.
  // bool octree_point_count_list_present_flag;
  /**
  * @brief オクトリーレベルの点のカウント情報の有無を取得
  * @details 本値は各オクトリーレベルの点のカウントが各ジオメトリスライスに含まれることを示す。
  * @return bool: TRUE: オクトリーレベルの点のカウントが各ジオメトリスライスに含まれる
  * @return bool: FALSE: オクトリーレベルの点のカウントが各ジオメトリスライスに含まれない
  */
  virtual bool getOctreePointCountListPresentFlag() const = 0;

  /**
  * @brief オクトリーレベルの点のカウント情報の有無を設定
  * @details 本値は各オクトリーレベルの点のカウントが各ジオメトリスライスに含まれることを示す。
  * @param[in] bool flag: TRUE: オクトリーレベルの点のカウントが各スライスに存在
  * @param[in] bool flag: FALSE: オクトリーレベルの点のカウントが各スライスに存在しない
  */
  virtual void setOctreePointCountListPresentFlag(bool flag) = 0;

protected:
  GeometryParameterSet() = default;
  virtual ~GeometryParameterSet() = default;
  explicit GeometryParameterSet(GeometryParameterSet&&) noexcept = default;

private:
  GeometryParameterSet(const GeometryParameterSet&) = delete;
  GeometryParameterSet& operator=(const GeometryParameterSet&) = delete;
  GeometryParameterSet& operator=(GeometryParameterSet&&) noexcept = delete;
};

//============================================================================
/**
* @brief ジオメトリブリックフッタクラス
* @details スライス毎のジオメトリコーディングに関するパラメータを保持するクラス
*/
class GeometryBrickFooter {
public:
  /**
  * @brief インスタンス生成
  * @details GeometryBrickFooterクラスのインスタンスを生成する
  * @return GeometryBrickFooter* 生成したGeometryBrickFooterインスタンスのポインタ
  */
  static GeometryBrickFooter* create();

  /**
  * @brief インスタンス解放
  * @details GeometryBrickFooterクラスのインスタンスを解放する
  */
  virtual void release() = 0;

  // int geom_num_points_minus1;
  /**
  * @brief スライス内に存在する実際のポイント数の取得
  * @details 本値はスライス内に存在する実際のポイント数を示します。
  * @return int: スライス内に存在する実際のポイント数
  */
  virtual int getGeomNumPointsMinus1() const = 0;
  /**
  * @brief スライス内に存在する実際のポイント数の設定
  * @details 本値はスライス内に存在する実際のポイント数を示します。
  * @param[in] int val: スライス内に存在する実際のポイント数
  */
  virtual void setGeomNumPointsMinus1(int val) = 0;

  // std::vector<int> octree_lvl_num_points_minus1;
  /**
  * @brief オクツリーレベル毎でデコード可能なポイント数の取得
  * @details 本値はオクツリーレベルごとにデコード可能なポイント数を示します。
  * @return Int32VectorAdapter&: オクツリーレベル毎でデコード可能なポイント数
  */
  virtual const Int32VectorAdapter& getOctreeLvlNumPointsMinus1() const = 0;
  /**
  * @brief オクツリーレベル毎でデコード可能なポイント数の取得
  * @details 本値はオクツリーレベルごとにデコード可能なポイント数を示します。
  * @return Int32VectorAdapter&: オクツリーレベル毎でデコード可能なポイント数
  */
  virtual Int32VectorAdapter& getOctreeLvlNumPointsMinus1() = 0;

protected:
  GeometryBrickFooter() = default;
  virtual ~GeometryBrickFooter() = default;
  explicit GeometryBrickFooter(GeometryBrickFooter&&) noexcept = default;

private:
  GeometryBrickFooter(const GeometryBrickFooter&) = delete;
  GeometryBrickFooter& operator=(const GeometryBrickFooter&) = delete;
  GeometryBrickFooter& operator=(GeometryBrickFooter&&) noexcept = delete;
};

//============================================================================
/**
* @note TODO.調査中(2)
* @brief  GeometryBrickHeaderクラス
* @details GeometryBrickHeader(Sliceに関するヘッダ)クラス
*/
class GeometryBrickHeader {
public:
/**
  * @brief インスタンス生成
  * @details GeometryBrickHeaderクラスのインスタンスを生成する
  * @return GeometryBrickHeader*: 生成したGeometryBrickHeaderインスタンスのポインタ
  */
  static GeometryBrickHeader* create();
  
  /**
  * @brief インスタンス解放
  * @details GeometryBrickHeaderクラスのインスタンスを解放する
  */
  virtual void release() = 0;

  // int geom_geom_parameter_set_id;
  // int slice_tag;
  // int geom_slice_id;
  /**
  * @brief GeometryBrickHeaderのジオメトリパラメータセットIDを取得
  * @details GeometryBrickHeaderのジオメトリパラメータセットIDを取得
  * @return int: GeometryBrickHeaderのジオメトリパラメータセットID
  */
  virtual int getGeomGeomParameterSetId() const = 0;
  
  /**
  * @brief GeometryBrickHeaderのジオメトリパラメータセットIDを設定
  * @details GeometryBrickHeaderのジオメトリパラメータセットIDを設定
  * @return int id: GeometryBrickHeaderのジオメトリパラメータセットID
  */
  virtual void setGeomGeomParameterSetId(int id) = 0;
  
  /**
  * @brief スライスタグを取得
  * @details 本値は該当スライスのタグを示します。
  * @return int: スライスのタグ
  */
  virtual int getSliceTag() const = 0;

  /**
  * @brief スライスタグを設定
  * @details 本値は該当スライスのタグを示します。
  * @param[in] int tag: スライスのタグ
  */
  virtual void setSliceTag(int tag) = 0;
  
  /**
  * @brief スライスIDを取得
  * @details 本値は該当スライスのIDを示します。
  * @return int: スライスID
  */
  virtual int getGeomSliceId() const = 0;

  /**
  * @brief スライスIDを設定
  * @details 本値は該当スライスのIDを示します。
  * @return int id: スライスID
  */
  virtual void setGeomSliceId(int id) = 0;

  // int frame_ctr_lsb;
  /**
  * @brief フレームカウンタのLSBを取得
  * @details 本値はシステムで用いるフレームカウンタのLSBを示します。
  * @return int: フレームカウンタのLSB
  */
  virtual int getFrameCtrLsb() const = 0;
  
  /**
  * @brief フレームカウンタのLSBを設定
  * @details 本値はシステムで用いるフレームカウンタのLSBを示します。
  * @param[in] int val: フレームカウンタのLSB
  */
  virtual void setFrameCtrLsb(int val) = 0;

  // Vec3<int> geomBoxOrigin;
  // int geom_box_log2_scale;
  /**
  * @brief スライスの原点を取得
  * @details 本値はスライスの原点を示します。
  * @return point_t&: スライスの原点
  */
  virtual const point_t& getGeomBoxOrigin() const = 0;
  
  /**
  * @brief スライスの原点を取得
  * @details 本値はスライスの原点を示します。
  * @return point_t&: スライスの原点
  */
  virtual point_t& getGeomBoxOrigin() = 0;

  /**
  * @brief スライスの原点を設定
  * @details 本値はスライスの原点を示します。
  * @param[in] point_t& val: スライスの原点
  */
  virtual void setGeomBoxOrigin(const point_t& val) = 0;

  /**
  * @brief スライスのスケールを取得
  * @details 本値はスライスのスライスのスケールを示します。
  * @return int: スライスのスケール
  */
  virtual int getGeomBoxLog2Scale() const = 0;

  /**
  * @brief スライスのスケールを設定
  * @details 本値はスライスのスライスのスケールを示します。
  * @param[in] int val: スライスのスケール
  */
  virtual void setGeomBoxLog2Scale(int val) = 0;

  // int geom_box_origin_bits_minus1;
  /**
  * @brief スライスの原点のビット数を取得
  * @details 本値はスライスの原点のビット数を示します。
  * @return int: スライスの原点のビット数
  */
  virtual int getGeomBoxOriginBitsMinus1() const = 0;

  /**
  * @brief スライスの原点のビット数を設定
  * @details 本値はスライスの原点のビット数を示します。
  * @param[in] int val: スライスの原点のビット数
  */
  virtual void setGeomBoxOriginBitsMinus1(int val) = 0;

  // (in stv axis order).
  // Vec3<int> gbhAngularOrigin;
  /**
  * @brief スライスの角度原点を取得
  * @details 本値はスライスの角度モードでの原点を示します。
  * スライスのバウンディングボックスからの相対値となります。
  * @return point_t&: スライスの角度原点
  */
  virtual const point_t& getGbhAngularOrigin() const = 0;

  /**
  * @brief スライスの角度原点を取得
  * @details 本値はスライスの角度モードでの原点を示します。
  * スライスのバウンディングボックスからの相対値となります。
  * @return point_t&: スライスの角度原点
  */
  virtual point_t& getGbhAngularOrigin() = 0;

  /**
   * @note TODO.(3) setterの間違いでは 
  * @brief スライスの角度原点を設定
  * @details 本値はスライスの角度モードでの原点を示します。
  * スライスのバウンディングボックスからの相対値となります。
  * @return point_t& val: スライスの角度原点
  */
  virtual void getGbhAngularOrigin(const point_t& val) = 0;

  // NB: this is only needed for the initial node size determination at
  //     the encoder
  // Vec3<int> rootNodeSizeLog2;
  /**
  * @brief ルートジオメトリノードのサイズを取得
  * @details 本値は元となるジオメトリ（スライス元）のノードのサイズを示します。
  * 注：この値はエンコーダーでの初期ノードサイズ決定のためのみに必要となります。
  * @return point_t&: ルートジオメトリサイズ
  */
  virtual const point_t& getRootNodeSizeLog2() const = 0;

  /**
  * @brief ルートジオメトリノードのサイズを取得
  * @details 本値は元となるジオメトリ（スライス元）のノードのサイズを示します。
  * 注：この値はエンコーダーでの初期ノードサイズ決定のためのみに必要となります。
  * @return point_t&: ルートジオメトリサイズ
  */
  virtual point_t& getRootNodeSizeLog2() = 0;

  /**
  * @brief ルートジオメトリノードのサイズを設定
  * @details 本値は元となるジオメトリ（スライス元）のノードのサイズを示します。
  * 注：この値はエンコーダーでの初期ノードサイズ決定のためのみに必要となります。
  * @param[in] point_t& val: ルートジオメトリサイズ
  */
  virtual void setRootNodeSizeLog2(const point_t& val) = 0;

  // Vec3<int> pgeom_resid_abs_log2_bits;
  /**
  * @note TODO.調査中(3)
  * @brief 予測ジオメトリの残差ビット数を取得
  * @details 本値は予測ジオメトリにおける残差ビット数を示します。
  * @return point_t&: 予測ジオメトリの残差ビット数
  */
  virtual const point_t& getPgeomResidAbsLog2Bits() const = 0;

  /**
  * @brief 予測ジオメトリの残差ビット数を取得
  * @details 本値は予測ジオメトリにおける残差ビット数を示します。
  * @return point_t&: 予測ジオメトリの残差ビット数
  */
  virtual point_t& getPgeomResidAbsLog2Bits() = 0;

  /**
  * @brief 予測ジオメトリの残差ビット数を設定
  * @details 本値は予測ジオメトリにおける残差ビット数を示します。
  * @param[in] point_t& val: 予測ジオメトリの残差ビット数
  */
  virtual void setPgeomResidAbsLog2Bits(const point_t& val) = 0;

  // mutable int maxRootNodeDimLog2;
  // NOTE: mutable
  /**
  * @brief ルートジオメトリノードの最大値を取得
  * @details 本値は元となるジオメトリ（スライス元）のノードのサイズの全次元中の最大値
  * を示します。
  * @return int: ルートジオメトリノードの最大値
  */
  virtual int getMaxRootNodeDimLog2() const = 0;
  /**
  * @brief ルートジオメトリノードの最大値を設定
  * @details 本値は元となるジオメトリ（スライス元）のノードのサイズの全次元中の最大値
  * を示します。
  * @param[in] int val: ルートジオメトリノードの最大値
  */
  virtual void setMaxRootNodeDimLog2(int val) = 0;

  // std::vector<int8_t> tree_lvl_coded_axis_list;
  /**
  * @note TODO.調査中(3)　Must！
  * @brief スライスのツリーレベルごとのxyz軸リスト（各軸の最大値）を取得
  * @details 本値はスライスのツリーレベルごとのxyz軸リスト（各軸の最大値）を示します。
  * @return Int8VectorAdapter&: スライスのツリーレベルごとのxyz軸リスト（各軸の最大値）
  */
  virtual const Int8VectorAdapter& getTreeLvlCodedAxisList() const = 0;

  /**
  * @brief スライスのツリーレベルごとのxyz軸リスト（各軸の最大値）を取得
  * @details 本値はスライスのツリーレベルごとのxyz軸リスト（各軸の最大値）を示します。
  * @return Int8VectorAdapter&: スライスのツリーレベルごとのxyz軸リスト（各軸の最大値）
  */
  virtual Int8VectorAdapter& getTreeLvlCodedAxisList() = 0;

  // int geom_slice_qp_offset;
  /**
  * @note TODO.調査中(3)　Must！
  * @brief スライス間QPのオフセットを取得
  * @details 本値はスライス間QP（量子化パラメータ）のオフセットを示します。
  * @return int: スライス間QPのオフセット
  */
  virtual int getGeomSliceQpOffset() const = 0;

  /**
  * @brief スライス間QPのオフセットを設定
  * @details 本値はスライス間QP（量子化パラメータ）のオフセットを示します。
  * @param[in] int val: スライス間QPのオフセット
  */
  virtual void setGeomSliceQpOffset(int val) = 0;

  // int geom_qp_offset_intvl_log2_delta;
  /**
  * @note TODO.調査中(3)　Must！
  * @brief 予測ジオメトリコーディングのブロックサイズオフセットを取得
  * @details 本値は予測ジオメトリコーディングのブロックサイズのオフセット（有効な場合）を示します。
  * @return int: 予測ジオメトリコーディングのブロックサイズオフセット
  */
  virtual int getGeomQpOffsetIntvlLog2Delta() const = 0;

  /**
  * @brief 予測ジオメトリコーディングのブロックサイズオフセットを設定
  * @details 本値は予測ジオメトリコーディングのブロックサイズのオフセット（有効な場合）を示します。
  * @param[in] int val: 予測ジオメトリコーディングのブロックサイズオフセット
  */
  virtual void setGeomQpOffsetIntvlLog2Delta(int val) = 0;

  // int geom_stream_cnt_minus1;
  /**
  * @brief オクツリーのエンコードに使用されるサブストリームの数を取得
  * @details 本値はオクツリーのエンコードに使用されるジオメトリサブストリーム
  * （並列コーディングに用いられる）の数を示します。
  * 例）値が 8 の場合、8つのサブストリームが生成、初期ツリー用に1つ、最後の7つのツリーレベルごとに
  * 1つずつ生成されます。
  * 値が1の場合は並列サブストリームは生成されません。
  * @return int: オクツリーのエンコードに使用されるサブストリームの数
  */
  virtual int getGeomStreamCntMinus1() const = 0;

  /**
  * @brief オクツリーのエンコードに使用されるサブストリームの数を設定
  * @details 本値はオクツリーのエンコードに使用されるジオメトリサブストリーム
  * （並列コーディングに用いられる）の数を示します。
  * 例）値が 8 の場合、8つのサブストリームが生成、初期ツリー用に1つ、最後の7つのツリーレベルごとに
  * 1つずつ生成されます。
  * 値が1の場合は並列サブストリームは生成されません。
  * @param[in] int val: オクツリーのエンコードに使用されるサブストリームの数
  */
  virtual void setGeomStreamCntMinus1(int val) = 0;

  // int trisoup_node_size_log2_minus2;
  /**
  * @brief トリソープジオメトリにおける三角形のノード数を取得
  * @details 本値はトリソープジオメトリにおける三角形のノード（再構築されたサーフェス）の数を
  * 示します。
  * @return int: トリソープジオメトリにおける三角形のノード数
  */
  virtual int getTrisoupNodeSizeLog2Minus2() const = 0;

  /**
  * @brief トリソープジオメトリにおける三角形のノード数を設定
  * @details 本値はトリソープジオメトリにおける三角形のノード（再構築されたサーフェス）の数を
  * 示します。
  * @return int val: トリソープジオメトリにおける三角形のノード数
  */
  virtual void setTrisoupNodeSizeLog2Minus2(int val) = 0;

  // int trisoup_sampling_value_minus1;
  /**
  * @brief 三角形ボクセル化処理によって生成される点の数を取得
  * @details 本値は三角形ボクセル化処理によって生成される点の数を制御します。
  * | 値     | 内容
  * |:-----:| -------------------------------|
  * | 0     ｜自動（デフォルト
  * | 1     ｜サブサンプリングなし
  * | N >=1 | ポイント・サンプリング周期
  * 自動モードは、生成されたポイント数がsliceMaxPointsで設定されたスライス制限を
  * 超えないような最小のサンプリング値を見つけます。
  * @return int: 三角形ボクセル化処理によって生成される点の数
  */
  virtual int getTrisoupSamplingValueMinus1() const = 0;

  /**
  * @brief 三角形ボクセル化処理によって生成される点の数を設定
  * @details 本値は三角形ボクセル化処理によって生成される点の数を制御します。
  * | 値     | 内容
  * |:-----:| -------------------------------|
  * | 0     ｜自動（デフォルト
  * | 1     ｜サブサンプリングなし
  * | N >=1 | ポイント・サンプリング周期
  * 自動モードは、生成されたポイント数がsliceMaxPointsで設定されたスライス制限を
  * 超えないような最小のサンプリング値を見つけます。
  * @param[in] int val: 三角形ボクセル化処理によって生成される点の数
  */
  virtual void setTrisoupSamplingValueMinus1(int val) = 0;

  // int num_unique_segments_minus1;
  /**
   * @note  TODO.調査中(3)
  * @brief スライス内のユニークなセグメント数を取得
  * @details 本値はスライス内のユニークなセグメント数を示します。
  * @return int: スライス内のユニークなセグメント数
  */
  virtual int getNumUniqueSegmentsMinus1() const = 0;

  /**
  * @brief スライス内のユニークなセグメント数を設定
  * @details 本値はスライス内のユニークなセグメント数を示します。
  * @param[in] int val: スライス内のユニークなセグメント数
  */
  virtual void setNumUniqueSegmentsMinus1(int val) = 0;

  // int num_unique_segments_bits_minus1;
  /**
  * @note  TODO.調査中(3)
  * @brief スライス内のユニークなセグメントのビット数を取得
  * @details 本値はスライス内のユニークなセグメントのビット数を示します。
  * @return int: スライス内のユニークなセグメントのビット数
  */
  virtual int getNumUniqueSegmentsBitsMinus1() const = 0;

  /**
  * @brief スライス内のユニークなセグメントのビット数を設定
  * @details 本値はスライス内のユニークなセグメントのビット数を示します。
  * @param[in] int val: スライス内のユニークなセグメントのビット数
  */
  virtual void setNumUniqueSegmentsBitsMinus1(int val) = 0;
  
  /**
  * @brief GeometryBrickFooterの実体を取得
  * @details 本値はGeometryBrickFooterの実体を示します。
  * データユニットの最後に表示される情報を示します。
  * @return GeometryBrickFooter&: GeometryBrickFooterの実体
  */
  virtual const GeometryBrickFooter& getFooter() const = 0;

  /**
  * @brief GeometryBrickFooterの実体を取得
  * @details 本値はGeometryBrickFooterの実体を示します。
  * データユニットの最後に表示される情報を示します。
  * @return GeometryBrickFooter&: GeometryBrickFooterの実体
  */
  virtual GeometryBrickFooter& getFooter() = 0;

  // bool entropy_continuation_flag;
  /**
  * @brief スライス間のエントロピー符号化状態を取得
  * @details 本値は同一フレーム内のスライス間のエントロピー符号化状態（コンテキスト値）
  * の伝播を制御を示します。
  * 有効にすると、（最初のスライスを除く）各スライスは前のスライスにコーディング依存します。
  * @return bool: TRUE: 各スライスは前のスライスにコーディング依存する
  * @return bool: FALSE: 各スライスは前のスライスにコーディング依存しない
  */
  virtual bool getEntropyContinuationFlag() const = 0;

  /**
  * @brief スライス間のエントロピー符号化状態を設定
  * @details 本値は同一フレーム内のスライス間のエントロピー符号化状態（コンテキスト値）
  * の伝播を制御を示します。
  * 有効にすると、（最初のスライスを除く）各スライスは前のスライスにコーディング依存します。
  * @param[in] bool flag: TRUE: 各スライスは前のスライスにコーディング依存する
  * @param[in] bool flag: FALSE: 各スライスは前のスライスにコーディング依存しない
  */
  virtual void setEntropyContinuationFlag(bool flag) = 0;

  // int prev_slice_id;
  /**
  * @brief ビットストリーム内の前のスライスIDを取得
  * @details 本値はビットストリーム内における前のスライスIDを示します。
  * @return int: ビットストリーム内の前のスライスID
  */
  virtual int getPrevSliceId() const = 0;

  /**
  * @brief ビットストリーム内の前のスライスIDを設定
  * @details 本値はビットストリーム内における前のスライスIDを示します。
  * @param[in] int id: ビットストリーム内の前のスライスID
  */
  virtual void setPrevSliceId(int id) = 0;

  // int pgeom_min_radius;
  /**
  * @brief 角度モードによる予測ジオメトリ符号化の最小半径を取得
  * @details 本値は角度モードによる予測ジオメトリ符号化の最小半径を示します。
  * @return int: 角度モードによる予測ジオメトリ符号化の最小半径
  */
  virtual int getPgeomMinRadius() const = 0;

  /**
  * @brief 角度モードによる予測ジオメトリ符号化の最小半径を設定
  * @details 本値は角度モードによる予測ジオメトリ符号化の最小半径を示します。
  * @param[in] int val: 角度モードによる予測ジオメトリ符号化の最小半径
  */
  virtual void setPgeomMinRadius(int val) = 0;

protected:
  GeometryBrickHeader() = default;
  virtual ~GeometryBrickHeader() = default;
  explicit GeometryBrickHeader(GeometryBrickHeader&&) noexcept = default;

private:
  GeometryBrickHeader(const GeometryBrickHeader&) = delete;
  GeometryBrickHeader& operator=(const GeometryBrickHeader&) = delete;
  GeometryBrickHeader& operator=(GeometryBrickHeader&&) noexcept = delete;
};

//============================================================================
// NB: when updating this, remember to update AttributeLods::isReusable(...)

/**
 * @brief AttributeParamterSetクラス
 * @details 点情報の属性値を保持するクラス
 */
class GPCCLIB_INTERFACE_SPEC AttributeParameterSet {
public:
  /**
  * @brief インスタンス生成
  * @details AttributeParameterSetクラスのインスタンスを生成する
  * @return AttributeParameterSet* 生成したAttributeParameterSetインスタンスのポインタ
  */
  static AttributeParameterSet* create();

  /**
  * @brief インスタンス解放
  * @details AttributeParameterSetクラスのインスタンスを解放する
  */
  virtual void release() = 0;

  // int aps_attr_parameter_set_id;
  // int aps_seq_parameter_set_id;
  // AttributeEncoding attr_encoding;

  /**
   * @brief パラメータセットIDを取得
   * @details パラメータセットIDを取得する
   * @return パラメータセットID
   */
  virtual int getApsAttrParameterSetId() const = 0;

  /**
   * @brief パラメータセットIDを設定
   * @details パラメータセットIDを設定する
   * @param[in] id: パラメータセットID
   */
  virtual void setApsAttrParameterSetId(int id) = 0;

  /**
   * @brief シーケンスIDを取得
   * @details シーケンスIDを取得する
   * @return シーケンスID
   */
  virtual int getApsSeqParameterSetId() const = 0;
  /**
   * @brief シーケンスIDを設定
   * @details シーケンスIDを設定する
   * @param[in] id: シーケンスID
   */
  virtual void setApsSeqParameterSetId(int id) = 0;

  /**
   * @brief 圧縮属性を取得
   * @details 圧縮属性を取得する
   * @return 圧縮属性
   */
  virtual AttributeEncoding getAttrEncoding() const = 0;
  /**
   * @brief 圧縮属性を設定
   * @brief 圧縮属性を設定する
   */
  virtual void setAttrEncoding(AttributeEncoding enc) = 0;

  //--- lifting/predicting transform parameters

  // LodDecimationMethod lod_decimation_type;
  // bool canonical_point_order_flag;
  // int num_pred_nearest_neighbours_minus1;
  // int max_num_direct_predictors;
  // bool direct_avg_predictor_disabled_flag;
  // int adaptive_prediction_threshold;
  // int intra_lod_search_range;
  // int inter_lod_search_range;

  /**
   * @brief LoDサンプリング種別を取得
   * @details LoDサンプリング種別を取得する
   * | 値
   * |:-:| ------------------------------------------------------- |
   * | 0 | デシメーションは行われない。
   * | 1 | 周期的な lodSubsamplingPeriod によるデシメーション
   * | 2 | lodSubsamplingPeriodの重心までの距離でデシメーションする。
   * @return LodDecimationMethod LoDサンプリング種別
   */
  virtual LodDecimationMethod getLodDecimationType() const = 0;

  /**
   * @brief LoDサンプリング種別を設定
   * @details LoDサンプリング種別を設定する
   * | 値
   * |:-:| ------------------------------------------------------- |
   * | 0 | デシメーションは行われない。
   * | 1 | 周期的な lodSubsamplingPeriod によるデシメーション
   * | 2 | lodSubsamplingPeriodの重心までの距離でデシメーションする。
   * @param[in] val: LoDサンプリング種別
   */
  virtual void setLodDecimationType(LodDecimationMethod val) = 0;

  /**
   * @brief アトリビュートの符号化の順序を取得
   * @details 本値はアトリビュートの符号化に使用される順序を制御します。
   * ジオメトリのデコード順は、LoD属性コーディングとlevelOfDetailCount=0(num_detail_levels_minus1=0)
   * の場合のみ使用可能。
   * @return bool: TRUE: 正準オーダー（デコードされたジオメトリ順）
   * @return bool: FALSE: モートンオーダー
   */
  virtual bool getCanonicalPointOrderFlag() const = 0;

  /**
   * @brief アトリビュートの符号化の順序を設定
   * @details 本値はアトリビュートの符号化に使用される順序を制御します。
   * ジオメトリのデコード順は、LoD属性コーディングとlevelOfDetailCount=0(num_detail_levels_minus1=0)
   * の場合のみ使用可能。
   * @param[in] bool flag: TRUE: 正準オーダー（デコードされたジオメトリ順）
   * @param[in] bool flag: FALSE: モートンオーダー
   */
  virtual void setCanonicalPointOrderFlag(bool flag) = 0;
  
  /**
  * @brief 予測に使用する最近傍の最大数を取得
  * @details 本値は予測に使用する最近傍の最大数を示します。
  * @return int: 予測に使用する最近傍の最大数
  */
  virtual int getNumPredNearestNeighboursMinus1() const = 0;

  /**
  * @brief 予測に使用する最近傍の最大数を設定
  * @details 本値は予測に使用する最近傍の最大数を示します。
  * @param[in] int val: 予測に使用する最近傍の最大数
  */
  virtual void setNumPredNearestNeighboursMinus1(int val) = 0;
  
  /**
  * @brief アトリビュートの直接予測に使用される最近の候補の最大数を取得
  * @details 本値はアトリビュートの直接予測に使用される最近の候補の最大数を示します。
  * @return int: アトリビュートの直接予測に使用される最近の候補の最大数
  */
  virtual int getMaxNumDirectPredictors() const = 0;

  /**
  * @brief アトリビュートの直接予測に使用される最近の候補の最大数を設定
  * @details 本値はアトリビュートの直接予測に使用される最近の候補の最大数を示します。
  * @return int val: アトリビュートの直接予測に使用される最近の候補の最大数
  */
  virtual void setMaxNumDirectPredictors(int val) = 0;

  /**
  * @brief 近隣平均予測が禁止されているかを取得
  * @details 本値は直接予測が呼び出された時に近隣平均予測の使用の許可状態を示します。
  * @return bool: TRUE: 直接予測が呼び出された時の近隣平均予測を禁止
  * @return bool: FALSE: 直接予測が呼び出された時の近隣平均予測を許可
  */
  virtual bool isDirectAvgPredictorDisabled() const = 0;

  /**
  * @brief 近隣平均予測が禁止されているかを設定
  * @details 本値は直接予測が呼び出された時に近隣平均予測の使用の許可状態を示します。
  * @param[in] bool disabled: TRUE: 直接予測が呼び出された時の近隣平均予測を禁止
  * @param[in] bool disabled: FALSE: 直接予測が呼び出された時の近隣平均予測を許可
  */
  virtual void setDirectAvgPredictorDisabled(bool disabled) = 0;
  
  /**
  * @note TODO.調査中(3)
  * @brief 直接予測を可能にする近傍アトリビュート値の差分の閾値を取得
  * @details 本値は平均で重みづけられた箇所の選択に対する直接予測を可能にする近傍アトリビュート値
  * の差分の閾値を示します。
  * bitDepthが8より大きい場合、閾値は2**(bitDepth - 8)でスケーリングされます。
  * @return int: 直接予測を可能にする近傍アトリビュート値の差分の閾値
  */
  virtual int getAdaptivePredictionThreshold() const = 0;

  /**
  * @brief 直接予測を可能にする近傍アトリビュート値の差分の閾値を設定
  * @details 本値は平均で重みづけられた箇所の選択に対する直接予測を可能にする近傍アトリビュート値
  * の差分の閾値を示します。
  * bitDepthが8より大きい場合、閾値は2**(bitDepth - 8)でスケーリングされます。
  * transformType=0 時のみ適用されます。
  * @param[in] int val: 直接予測を可能にする近傍アトリビュート値の差分の閾値
  */
  virtual void setAdaptivePredictionThreshold(int val) = 0;
  
  /**
  * @brief 同じ詳細レベル(LoD)内の最近傍を検索する範囲を取得
  * @details 本値は同じ詳細レベル(LoD)内の最近傍を検索する範囲を示します。
  * -1: 全範囲検索
  *  0: 詳細レベル内の検索無効
  * @return int: 同じ詳細レベル(LoD)内の最近傍を検索する範囲
  */
  virtual int getIntraLodSearchRange() const = 0;

  /**
  * @brief 同じ詳細レベル(LoD)内の最近傍を検索する範囲を設定
  * @details 本値は同じ詳細レベル(LoD)内の最近傍を検索する範囲を示します。
  * -1: 全範囲検索
  *  0: 詳細レベル内の検索無効
  * @param[in] int val: 同じ詳細レベル(LoD)内の最近傍を検索する範囲
  */
  virtual void setIntraLodSearchRange(int val) = 0;

  /**
  * @brief 異なる詳細レベル(LoD)間の最近傍を検索する範囲を取得
  * @details 本値は異なる詳細レベル(LoD)間の最近傍を検索する範囲を示します。
  * -1: 全範囲検索
  * @return int: 異なる詳細レベル(LoD)間の最近傍を検索する範囲
  */
  virtual int getInterLodSearchRange() const = 0;

  /**
  * @brief 異なる詳細レベル(LoD)間の最近傍を検索する範囲を取得
  * @details 本値は異なる詳細レベル(LoD)間の最近傍を検索する範囲を示します。
  * -1: 全範囲検索
  * @return int: 異なる詳細レベル(LoD)間の最近傍を検索する範囲
  */
  virtual void setInterLodSearchRange(int val) = 0;

  // Vec3<int32_t> lodNeighBias;
  /**
  * @note TODO.調査中(3)
  * @brief 最近傍の重み付け用バイアス係数を取得
  * @details 本値はLoD生成および重み付け処理中に最近傍を重み付けするために使用される
  * 第1、第2および第3のジオメトリ軸に対応する3つのバイアス係数のセットを示します。 
  * 1,1,1: バイアス無し
  * @return int: 最近傍の重み付け用バイアス係数
  */
  virtual const point_t& getLodNeighBias() const = 0;

  /**
  * @brief 最近傍の重み付け用バイアス係数を取得
  * @details 本値はLoD生成および重み付け処理中に最近傍を重み付けするために使用される
  * 第1、第2および第3のジオメトリ軸に対応する3つのバイアス係数のセットを示します。 
  * 1,1,1: バイアス無し
  * @return int: 最近傍の重み付け用バイアス係数
  */
  virtual point_t& getLodNeighBias() = 0;

  /**
  * @brief 最近傍の重み付け用バイアス係数を設定
  * @details 本値はLoD生成および重み付け処理中に最近傍を重み付けするために使用される
  * 第1、第2および第3のジオメトリ軸に対応する3つのバイアス係数のセットを示します。 
  * 1,1,1: バイアス無し
  * @param[in] point_t& val: 最近傍の重み付け用バイアス係数
  */
  virtual void setLodNeighBias(const point_t& val) = 0;

  // prediction.
  // int intra_lod_prediction_skip_layers;
  /**
  * @brief イントラ予測を無効にするLoDの数を取得
  * @details 本値はイントラ予測を無効にするLoDの数を示します。 
  * AttributeEncoding transformType=0`にのみ適用されます。
  * -1: すべての詳細レベルで無効
  *  0: すべての詳細レベルで有効
  *  n: 最も細かいn個の詳細レベルで無効
  * @return int: イントラ予測を無効にするLoDの数
  */
  virtual int getIntraLodPredictionSkipLayers() const = 0;

  /**
  * @brief イントラ予測を無効にするLoDの数を設定
  * @details 本値はイントラ予測を無効にするLoDの数を示します。 
  * AttributeEncoding transformType=0`にのみ適用されます。
  * -1: すべての詳細レベルで無効
  *  0: すべての詳細レベルで有効
  *  n: 最も細かいn個の詳細レベルで無効
  * @param[in] int val: イントラ予測を無効にするLoDの数
  */
  virtual void setIntraLodPredictionSkipLayers(int val) = 0;

  // to be skipped.
  /**
  * @brief イントラ予測を無効にするLoDの定数
  * @details 本値はイントラ予測を無効にするLoDの数の定数を示します。 
  * 本値をintra_lod_prediction_skip_layersに指定するとすべてのレイヤー
  * のイントラ予測がスキップされます。
  */
  static const int kSkipAllLayers = 0x7fffffff;

  // bool inter_component_prediction_enabled_flag;
  // bool last_component_prediction_enabled_flag;
  /**
  * @note TODO.調査中(3)
  * @brief アトリビュートのコンポーネント予測の許可状態の取得
  * @details 本値はアトリビュート残差予測において、ループ内のコンポーネント内からの予測の使用
  * を行うかを示します。  
  * 有効にすると、2次的な属性残差（例：赤/青）は主成分（例：緑）から予測されます。
  * transformType=0 と attribute=color 時にのみ適用されます。
  * @return bool: TRUE: アトリビュートのコンポーネント内からの予測を許可
  * @return bool: FALSE: アトリビュートのコンポーネント内からの予測を許可しない
  */
  virtual bool isInterComponentPredictionEnabled() const = 0;

  /**
  * @brief アトリビュートのコンポーネント予測の許可状態の設定
  * @details 本値はアトリビュート残差予測において、ループ内のコンポーネント内からの予測の使用
  * を行うかを示します。  
  * 有効にすると、2次的な属性残差（例：赤/青）は主成分（例：緑）から予測されます。
  * transformType=0 と attribute=color 時にのみ適用されます。
  * @param[in] bool enabled: TRUE: アトリビュートのコンポーネント内からの予測を許可
  * @param[in] bool enabled: FALSE: アトリビュートのコンポーネント内からの予測を許可しない
  */
  virtual void setInterComponentPredictionEnabled(bool enabled) = 0;

  /**
  * @note TODO.調査中(3)
  * @brief アトリビュートの最後のコンポーネント予測の許可状態の取得
  * @details 本値はアトリビュート係数のループ内のコンポーネント内からの予測の使用 
  * を行うかを示します。  
  * 有効にすると、2次的な属性の最後のコンポーネント（例：Cr）の係数は単純なモデルに従って
  * 対応する最初のコンポーネント（例：Cb）から予測されます。
  * transformType=0 と attribute=color 時にのみ適用されます。
  * @return bool: TRUE: アトリビュートのコンポーネント内からの予測を許可
  * @return bool: FALSE: アトリビュートのコンポーネント内からの予測を許可しない
  */
  virtual bool isLastComponentPredictionEnabled() const = 0;

  /**
  * @brief アトリビュートの最後のコンポーネント予測の許可状態の設定
  * @details 本値はアトリビュート係数のループ内のコンポーネント内からの予測の使用 
  * を行うかを示します。  
  * 有効にすると、2次的な属性の最後のコンポーネント（例：Cr）の係数は単純なモデルに従って
  * 対応する最初のコンポーネント（例：Cb）から予測されます。
  * transformType=0 と attribute=color 時にのみ適用されます。
  * @param[in] bool enabled: TRUE: アトリビュートのコンポーネント内からの予測を許可
  * @param[in] bool enabled: FALSE: アトリビュートのコンポーネント内からの予測を許可しない
  */
  virtual void setLastComponentPredictionEnabled(bool enabled) = 0;

  // bool pred_weight_blending_enabled_flag;
  /**
  * @note TODO.調査中(3)
  * @brief 近傍予測から得られる距離のブレンド制御可否の取得
  * @details 本値は近傍間の相対距離に従って3つの近傍予測から得られる重み付けられた距離の
  * ブレンドを制御します。
  * transformType=0 時にのみ適用されます。
  * @return bool: TRUE: 近傍予測から得られる距離のブレンド可能
  * @return bool: FALSE: 近傍予測から得られる距離のブレンド無効
  */
  virtual bool isPredWeightBlendingEnabled() const = 0;

  /**
  * @brief 近傍予測から得られる距離のブレンド制御可否の設定
  * @details 本値は近傍間の相対距離に従って3つの近傍予測から得られる重み付けられた距離の
  * ブレンドを制御します。
  * transformType=0 時にのみ適用されます。
  * @param[in] bool enabled: TRUE: 近傍予測から得られる距離のブレンド可能
  * @param[in] bool enabled: FALSE: 近傍予測から得られる距離のブレンド無効
  */
  virtual void setPredWeightBlendingEnabled(bool enabled) = 0;

  // int num_detail_levels_minus1;
  /**
  * @note TODO.調査中(3) マニュアル（LoDのアトリビュート数）とコメント（The number of refinement layers）で不一致 
  * @brief LoDのレイヤ数を取得
  * @details 本値はLoDのレイヤ数を示します。
  * @return int: LoDのアトリビュート数
  */
  virtual int getNumDetailLevelsMinus1() const = 0;
  
  /**
  * @brief LoDのレイヤ数を設定
  * @details 本値はLoDのレイヤ数を示します。
  * @param[in] int val: LoDのアトリビュート数
  */
  virtual void setNumDetailLevelsMinus1(int val) = 0;

  // std::vector<int> lodSamplingPeriod;
  /**
   * @brief LoDサンプリング周期を取得
   * @details LoDサンプリング周期を取得する
   * @return LoDサンプリング周期の実体
   */
  virtual const Int32VectorAdapter& getLodSamplingPeriod() const = 0;

  /**
   * @brief LoDサンプリング周期を取得
   * @details LoDサンプリング周期を取得する
   * @return LoDサンプリング周期の実体
   */
  virtual Int32VectorAdapter& getLodSamplingPeriod() = 0;

  // int dist2;
  // bool aps_slice_dist2_deltas_present_flag;
  /**
  * @brief LoD生成に使用される初期二乗距離を取得
  * @details 本値は連続するLoD生成に使用される初期二乗距離を示します。
  * @return int: LoD生成に使用される初期二乗距離
  */
  virtual int getDist2() const = 0;

  /**
  * @brief LoD生成に使用される初期二乗距離を設定
  * @details 本値は連続するLoD生成に使用される初期二乗距離を示します。
  * @param[in] int val: LoD生成に使用される初期二乗距離を取得
  */
  virtual void setDist2(int val) = 0;

  /**
  * @brief スライスごとの QP 値のシグナリングを有効にするフラグの取得
  * @details 本値はスライスごとの QP 値のシグナリングを有効にするフラグを示します。
  * @return bool int: TRUE: スライスごとの QP 値のシグナリング有効
  * @return bool int: TRUE: スライスごとの QP 値のシグナリング無効
  */
  virtual bool getApsSliceDist2DeltasPresentFlag() const = 0;

  /**
  * @brief スライスごとの QP 値のシグナリングを有効にするフラグの設定
  * @details 本値はスライスごとの QP 値のシグナリングを有効にするフラグを示します。
  * @param[in] bool int: TRUE: スライスごとの QP 値のシグナリング有効
  * @param[in] bool int: FALSE: スライスごとの QP 値のシグナリング無効
  */
  virtual void setApsSliceDist2DeltasPresentFlag(bool flag) = 0;

  // int init_qp_minus4;
  // int aps_chroma_qp_offset;
  // bool aps_slice_qp_deltas_present_flag;
  /**
  * @brief 輝度のQPの初期値を取得
  * @details 本値は輝度アトリビュートの量子化パラメータQPの初期値を示します。
  * attribute=colourの場合のみ適用されます。
  * @return int: QPの初期値
  */
  virtual int getInitQpMinus4() const = 0;

  /**
  * @brief 輝度のQPの初期値を設定
  * @details 本値は輝度アトリビュートの量子化パラメータQPの初期値を示します。
  * attribute=colourの場合のみ適用されます。
  * @param[in] int val: QPの初期値
  */
  virtual void setInitQpMinus4(int val) = 0;

  /**
  * @brief 彩度のQPを取得
  * @details 本値は彩度アトリビュートの量子化パラメータQPを示します。
  * attribute=colourの場合のみ適用されます。
  * @return int: QP
  */
  virtual int getApsChromaQpOffset() const = 0;

  /**
  * @brief 彩度のQPを設定
  * @details 本値は彩度アトリビュートの量子化パラメータQPを示します。
  * attribute=colourの場合のみ適用されます。
  * @param[in] int val: QP
  */
  virtual void setApsChromaQpOffset(int val) = 0;
  
  /**
  * @note TODO.調査中(3)
  * @brief スライスごとのQP値の伝達の有効無効フラグを取得
  * @details 本値はスライスごとのQP値の伝達の有効無効フラグを示します。
  * @return bool: TRUE: スライスごとのQP値の伝達の有効
  * @return bool: FALSE: スライスごとのQP値の伝達の無効
  */
  virtual bool getApsSliceQpDeltasPresenFlag() const = 0;

  /**
  * @brief スライスごとのQP値の伝達の有効無効フラグを設定
  * @details 本値はスライスごとのQP値の伝達の有効無効フラグを示します。
  * @param[in] bool flag: TRUE: スライスごとのQP値の伝達の有効
  * @param[in] bool flag: FALSE: スライスごとのQP値の伝達の無効
  */
  virtual void setApsSliceQpDeltasPresenFlag(bool flag) = 0;

  // bool raht_prediction_enabled_flag;
  // int raht_prediction_threshold0;
  // int raht_prediction_threshold1;
  /**
  * @note TODO.調査中(3) 
  * @brief RAHT予測の有効/無効を取得
  * @details 本値はRegion Adaptive Hierarchical Transform(RAHT)予測の有効/無効を示します。
  * RAHT予測はトランスフォームツリー内の隣接する親ノードのDC値を空間的にアップサンプリングして、
  * RAHT係数のトランスフォームドメイン予測を使用するように制御します。
  * @return bool: TRUE: RAHT予測有効
  * @return bool: FALSE: RAHT予測無効
  */
  virtual bool isRahtPredictionEnabled() const = 0;

  /**
  * @brief RAHT予測の有効/無効を設定
  * @details 本値はRegion Adaptive Hierarchical Transform(RAHT)予測の有効/無効を示します。
  * RAHT予測はトランスフォームツリー内の隣接する親ノードのDC値を空間的にアップサンプリングして、
  * RAHT係数のトランスフォームドメイン予測を使用するように制御します。
  * @param[in] bool enabled: TRUE: RAHT予測有効
  * @param[in] bool enabled: FALSE: RAHT予測無効
  */
  virtual void setRahtPredictionEnabled(bool enabled) = 0;
  
  /**
  * @brief RAHT予測の閾値0を取得
  * @details 本値はRAHT係数のドメイン予測変換の有効/無効の判定に使用される
  * ブロックごとの閾値を示します。
  * この閾値は存在しなければならない親近傍点の数を指定します。
  * @return int: ブロック毎のRAHT係数のドメイン予測変換を有効とする閾値0
  */
  virtual int getRahtPredictionThreshold0() const = 0;

  /**
  * @brief RAHT予測の閾値0を設定
  * @details 本値はRAHT係数のドメイン予測変換の有効/無効の判定に使用される
  * ブロックごとの閾値を示します。
  * この閾値は存在しなければならない親近傍点の数を指定します。
  * @param[in] int val: ブロック毎のRAHT係数のドメイン予測変換を有効とする閾値0
  */
  virtual void setRahtPredictionThreshold0(int val) = 0;

  /**
  * @brief RAHT予測の閾値1を取得
  * @details 本値はRAHT係数のドメイン予測変換の有効/無効の判定に使用される
  * ブロックごとの閾値を示します。
  * この閾値は存在しなければならない近傍点の数を指定します。
  * @return int: ブロック毎のRAHT係数のドメイン予測変換を有効とする閾値1
  */
  virtual int getRahtPredictionThreshold1() const = 0;

  /**
  * @brief RAHT予測の閾値1を設定
  * @details 本値はRAHT係数のドメイン予測変換の有効/無効の判定に使用される
  * ブロックごとの閾値を示します。
  * この閾値は存在しなければならない近傍点の数を指定します。
  * @param[in] int val: ブロック毎のRAHT係数のドメイン予測変換を有効とする閾値1
  */
  virtual void setRahtPredictionThreshold1(int val) = 0;

  // bool scalable_lifting_enabled_flag;
  // int max_neigh_range_minus1;
  /**
  * @brief 空間的にスケーラブルなアトリビュートエンコーディングの有効/無効を取得
  * @details 本値は空間的にスケーラブルなアトリビュートエンコーディングの有効/無効を示します。
  * このオプションは以下の時に有効となります。
  * transformType=2
  * positionQpMultiplierLog2=3
  * lodDecimator=0
  * trisoupNodeSizeLog2=0
  * @return bool: TRUE: 空間的にスケーラブルなアトリビュートエンコーディング有効
  * @return bool: FALSE: 空間的にスケーラブルなアトリビュートエンコーディング無効
  */
  virtual bool isScalableLiftingEnabled() const = 0;

  /**
  * @brief 空間的にスケーラブルなアトリビュートエンコーディングの有効/無効を設定
  * @details 本値は空間的にスケーラブルなアトリビュートエンコーディングの有効/無効を示します。
  * このオプションは以下の時に有効となります。
  * transformType=2
  * positionQpMultiplierLog2=3
  * lodDecimator=0
  * trisoupNodeSizeLog2=0
  * @param[in] bool enabled: TRUE: 空間的にスケーラブルなアトリビュートエンコーディング有効
  * @param[in] bool enabled: FALSE: 空間的にスケーラブルなアトリビュートエンコーディング無効
  */
  virtual void setScalableLiftingEnabled(bool enabled) = 0;

  /**
  * @brief 予測に用いる隣接点との距離範囲を取得
  * @details 本値は点とその予測に使用される隣接点との距離を制限します。
  * 最大距離はノードの対角線の単位で表され、現在の詳細レベルに応じてスケーリングされます。
  * @return int: 予測に用いる隣接点との距離範囲
  */
  virtual int getMaxNeighRangeMinus1() const = 0;

  /**
  * @brief 予測に用いる隣接点との距離範囲を設定
  * @details 本値は点とその予測に使用される隣接点との距離を制限します。
  * 最大距離はノードの対角線の単位で表され、現在の詳細レベルに応じてスケーリングされます。
  * @param[in] int val: 予測に用いる隣接点との距離範囲
  */
  virtual void setMaxNeighRangeMinus1(int val) = 0;

  // pseudo-spherical domain
  // bool spherical_coord_flag;
  /**
  * @brief 球面領域への変換の有効/無効を取得
  * @details 本値はアトリビュートコーディングで使用される点座標をデカルト領域から
  * スケーリングされた球面領域への変換を制御します。
  * @return bool: TRUE: 球面領域への変換有効
  * @return bool: FALSE: 球面領域への変換無効
  */
  virtual bool getSphericalCoordFlag() const = 0;

  /**
  * @brief 球面領域への変換の有効/無効を設定
  * @details 本値はアトリビュートコーディングで使用される点座標をデカルト領域から
  * スケーリングされた球面領域への変換を制御します。
  * @param[in] bool flag: TRUE: 球面領域への変換有効
  * @param[in] bool flag: FALSE: 球面領域への変換無効
  */
  virtual void setSphericalCoordFlag(bool flag) = 0;

  // Vec3<int> attr_coord_scale;
  /**
  * @note TODO.調査中(3)
  * @brief 領域変換のスケールファクター取得
  * @details 本値は(半径, 角度, レーザーID) 領域への変換のスケールファクターを示します。
  * @return point_t&: 半径, 角度, レーザーID領域への変換のスケールファクター
  */
  virtual const point_t& getAttrCoordScale() const = 0;

  /**
  * @brief 領域変換のスケールファクター取得
  * @details 本値は(半径, 角度, レーザーID) 領域への変換のスケールファクターを示します。
  * @return point_t&: 半径, 角度, レーザーID領域への変換のスケールファクター
  */
  virtual point_t& getAttrCoordScale() = 0;

  /**
  * @brief 領域変換のスケールファクター設定
  * @details 本値は(半径, 角度, レーザーID) 領域への変換のスケールファクターを示します。
  * @return point_t&: 半径, 角度, レーザーID領域への変換のスケールファクター
  */
  virtual void setAttrCoordScale(const point_t& val) = 0;

  //bool raw_attr_variable_len_flag;
  /**
  * @brief アトリビュートが固定幅としてコード化されているかを取得
  * @details 本値はアトリビュートが固定幅としてコード化されているか可変長として
  * コード化されているかを示します。
  * @return bool: TRUE: アトリビュートが固定幅としてコード化されている
  * @return bool: FALSE: アトリビュートが可変長としてコード化されている
  */
  virtual bool getRawAttrVariableLenFlag() const = 0;

  /**
  * @brief アトリビュートが固定幅としてコード化されているかを設定
  * @details 本値はアトリビュートが固定幅としてコード化されているか可変長として
  * コード化されているかを示します。
  * @param bool flag: TRUE: アトリビュートが固定幅としてコード化されている
  * @param bool flag: FALSE: アトリビュートが可変長としてコード化されている
  */
  virtual void setRawAttrVariableLenFlag(bool flag) = 0;

protected:
  AttributeParameterSet() = default;
  virtual ~AttributeParameterSet() = default;
  explicit AttributeParameterSet(AttributeParameterSet&&) noexcept = default;

private:
  AttributeParameterSet(const AttributeParameterSet&) = delete;
  AttributeParameterSet& operator=(const AttributeParameterSet&) = delete;
  AttributeParameterSet& operator=(AttributeParameterSet&&) = delete;
};

//============================================================================

} // namespace gpcclib
