/**
* @file encoder/params.hpp
* @brief Encoderクラスのパラメータが定義されたヘッダ
* @author KDDI CORPORATION
* @details Encoderクラスのパラメータが定義されたヘッダ。詳細な内容は各クラスメソッドなどを参照の事
*/
#pragma once

#include "gpcclib/def.hpp"
#include "gpcclib/params.hpp"

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

//============================================================================
/**
* @brief 再構築データの再カラーリングに関するパラメータの構造体
* @details 再構築データの再カラーリングに関するパラメータの構造体
*/
struct RecolourParams {
  //! @note TODO.調査中　再カラーリング時の距離で重み付けした近接点の色情報の平均値のオフセット
  //! 前方リストのポイントとターゲット間の距離がゼロの場合に、無限の重み付けを避けるための距離オフセット
  double distOffsetFwd;
  //! @note TODO.調査中　再カラーリング時の距離で重み付けした近接点の色情報の平均値のオフセット
  //! 後方リストのポイントとターゲット間の距離がゼロの場合に、無限の重みづけを避けるための距離オフセット
  double distOffsetBwd;
  //! 前方リストに入るための、ターゲットからのソースポイントの距離の2乗の最大許容値
  double maxGeometryDist2Fwd;
  //! 後方リストに入るための、ターゲットからのソースポイントの距離の2乗の最大許容値
  double maxGeometryDist2Bwd;
  //! 前方リストに含まれるソースポイントの属性値の2乗の差分（ターゲットからの）の最大許容値
  double maxAttributeDist2Fwd;
  //! 後方リストに含まれるソースポイントの属性値の2乗の差分（ターゲットからの）の最大許容値
  double maxAttributeDist2Bwd;
  //! 最適なアトリビュート転送のためのアトリビュート空間サーチ範囲
  int searchRange;
  //! 前方リストを作成するためにターゲット点の近傍で使用されたソース点の数
  int numNeighboursFwd;
  //! 後方リストを作成するためにターゲット点の近傍で使用されたソース点の数
  int numNeighboursBwd;

  //! 前方リストには距離加重平均を使用するかを示す
  bool useDistWeightedAvgFwd;
  //! 後方リストには距離加重平均を使用するかを示す
  bool useDistWeightedAvgBwd;
  //! 同一のソースポイントが存在する場合は前方リストを使用しないことを示す
  bool skipAvgIfIdenticalSourcePointPresentFwd;
  //! 同一のソースポイントが存在する場合は後方リストを使用しないことを示す
  bool skipAvgIfIdenticalSourcePointPresentBwd;
};

//============================================================================
/**
* @brief 入力PointCloudのパーティション分割方法
* @details 入力PointCloudデータをslices/tilesとしてパーティション分割する方法の列挙
*/
enum class PartitionMethod
{
  // Don't partition input
  //! 分割禁止
  kNone = 0,

  // Partition according to uniform geometry
  //! 均一ジオメトリーとして分割
  kUniformGeom = 2,

  // Partition according to the depth of octree
  //! octreeの深さによる分割
  kOctreeUniform = 3,

  // TBD
  //! TBD
  kUniformSquare = 4,

  // Paritition into n-point slices
  //! n-pointのsliceに分割
  kNpoints = 5,
};

//============================================================================
/**
* @brief パーティションパラメータ構造体
* @details 入力PointCloudデータをslices/tilesとしてパーティション分割する際のパラメータ
*/
struct PartitionParams {
  // Method for partitioning a point cloud
  //! パーティション分割方法
  PartitionMethod method;

  // Depth of octree used in partitioning
  //! octree深さ
  int octreeDepth;

  // Maximum number of points per slice
  //! sliceの最大ポイント数
  int sliceMaxPoints;

  // Minimum number of points per slice
  //! sliceの最小ポイント数
  int sliceMinPoints;

  // Baseline tile width. (0 => disabled)
  //! 分割する立方体のサイズ。0: Disable
  int tileSize;
};

//============================================================================
/**
* @brief Quad-Tree、Binary-Treeパーティションパラメータ構造体
* @details Quad-Tree、Binary-Treeパーティションに関するパラメータを格納する
*/
struct QtBtParameters {
  // maximum number of qtbt partitions before performing octree partitioning.
  //! octreeパーティション実施前のQuad-Tree、Binary-Treeパーティションの最大数
  int maxNumQtBtBeforeOt;

  // minimum size of qtbt partitions.
  //! Quad-Tree、Binary-Treeパーティションの最小数
  int minQtbtSizeLog2;
  //! trisoupコーディングの有効/無効
  bool trisoupEnabled;
  //! アンギュラーモードが有効な場合に分割する暗黙のQTBTのルールの微調整の有効/無効
  bool angularTweakEnabled;
  //! ノードサイズから垂直ノードを増やしたいかを示す整数
  int angularMaxNodeMinDimLog2ToSplitV;
  //! 取得したい最大垂直/水平比のlog2を提供する整数
  int angularMaxDiffToSplitZ;
};

//----------------------------------------------------------------------------
/**
* @brief octreeエンコードのオプション構造体
* @details octreeエンコードのオプション構造体
*/
struct OctreeEncOpts {
  //! Quad-Tree、Binary-Treeパーティションパラメータ構造体
  QtBtParameters qtbt;

  // Method used to derive in-tree quantisation parameters
  /**
  * @brief ツリー内量子化パラメータの導出方法
  * @details ツリー内量子化パラメータの導出方法
  */
  enum class QpMethod
  {
    //! 0: 均一
    kUniform = 0,
    //! 1: ランダム
    kRandom = 1,
    //! 2: ノード点密度
    kByDensity = 2,
  } qpMethod;

  // Tree depth at which to apply geometry quantisation
  //! ジオメトリ量子化を適用するツリーの深さ
  int qpOffsetDepth;

  // Node size (rather than depth) at which to apply geometry quantisation
  //! ジオメトリの定量化を適用する（深さではなく）ノードサイズ
  int qpOffsetNodeSizeLog2;
};

//=============================================================================
/**
* @brief 予測ジオメトリーエンコードのオプション
* @details 予測ジオメトリーエンコードのオプション構造体
*/
struct PredGeomEncOpts {
  //! ソートモード列挙
  enum SortMode
  {
    //! ソートしない
    kNoSort,
    //! モートンソート
    kSortMorton,
    //! azimuth(方位角)ソート
    kSortAzimuth,
    //! radius(半径)ソート
    kSortRadius
  } sortMode;

  // limit on number of points per tree
  //! ツリーごとのポイント数制限
  int maxPtsPerTree;

  // Reciprocal bin width used in azimuthal sorting.
  //  0 => full precision
  //! 方位角ソートで使用される逆バイナリ幅。0: フル精度
  int azimuthSortRecipBinWidth;
};

//============================================================================
// AttributeBrickHeader のインスタンスは EncoderAttributeParams で生成。
/**
* @note TODO.調査中　ふさわしい説明
* @brief AttributeBrickHeaderクラス
* @details AttributeBrickHeaderクラス。AttributeBrickHeaderのインスタンスはEncoderAttributeParams で生成
*/
class GPCCLIB_INTERFACE_SPEC AttributeBrickHeader {
public:
  /**
  * @note TODO.調査中
  * @brief アトリビュートレイヤの輝度情報の量子化パラメータ
  * @details アトリビュートレイヤの輝度情報の量子化パラメータ
  * @return attr_layer_qp_delta_luma(int32VectorAdapter型)を返す
  */
  virtual const Int32VectorAdapter& getAttrLayerQpDeltaLuma() const = 0;

  /**
  * @note TODO.調査中
  * @brief アトリビュートレイヤの輝度情報の量子化パラメータ
  * @details アトリビュートレイヤの輝度情報の量子化パラメータ
  * @return attr_layer_qp_delta_luma(int32VectorAdapter型)を返す
  */
  virtual Int32VectorAdapter& getAttrLayerQpDeltaLuma() = 0;
  /**
  * @note TODO.調査中
  * @brief アトリビュートレイヤの色度情報の量子化パラメータ
  * @details アトリビュートレイヤの色度情報の量子化パラメータ
  * @return attr_layer_qp_delta_chroma(int32VectorAdapter型)を返す
  */
  virtual const Int32VectorAdapter& getAttrLayerQpDeltaChroma() const = 0;
  /**
  * @note TODO.調査中
  * @brief アトリビュートレイヤの色度情報の量子化パラメータ
  * @details アトリビュートレイヤの色度情報の量子化パラメータ
  * @return attr_layer_qp_delta_chroma(int32VectorAdapter型)を返す
  */
  virtual Int32VectorAdapter& getAttrLayerQpDeltaChroma() = 0;

protected:
  AttributeBrickHeader() = default;
  virtual ~AttributeBrickHeader() = default;
  explicit AttributeBrickHeader(AttributeBrickHeader&&) noexcept = default;

private:
  AttributeBrickHeader(const AttributeBrickHeader&) = delete;
  AttributeBrickHeader& operator=(const AttributeBrickHeader&) = delete;
  AttributeBrickHeader&
  operator=(AttributeBrickHeader&&) noexcept = delete;
};

//============================================================================
/**
* @brief EncoderAttributeParamsクラス
* @details エンコーダのアトリビュートに関するパラメータを保持するクラス
*/
class GPCCLIB_INTERFACE_SPEC EncoderAttributeParams {
public:
  
  /**
  * @brief インスタンス生成
  * @details EncoderAttributeParamsクラスのインスタンスを生成する
  * @return EncoderAttributeParams* 生成したEncoderAttributeParamsインスタンスのポインタ
  */
  static EncoderAttributeParams* create();

  /**
  * @brief インスタンス解放
  * @details EncoderAttributeParamsクラスのインスタンスを解放する
  */
  virtual void release() = 0;

  // NB: this only makes sense for setting configurable parameters
  /**
  * @brief AttributeBrickHeaderを取得
  * @details AttributeBrickHeaderの実体を返す。注：設定可能なパラメータを設定する場合にのみ意味があります。
  * @return AttributeBrickHeader& AttributeBrickHeaderの実体
  */
  virtual const AttributeBrickHeader& getAbh() const = 0;
  
  /**
  * @brief AttributeBrickHeaderを取得
  * @details AttributeBrickHeaderの実体を返す。注：設定可能なパラメータを設定する場合にのみ意味があります。
  * @return AttributeBrickHeader& AttributeBrickHeaderの実体
  */
  virtual AttributeBrickHeader& getAbh() = 0;

  // Threshold for choosing dist2 out of the population of nearest neighbour
  // distances.
  /**
  * @brief 最近傍距離の母集団から dist2 を選択するための閾値を取得
  * @details 最近傍距離の母集団から dist2 を選択するための閾値を取得
  * @return 最近傍距離の母集団から dist2 を選択するための閾値
  */
  virtual float getDist2PercentileEstimate() const = 0;
  
  /**
  * @brief 最近傍距離の母集団から dist2 を選択するための閾値を設定
  * @details 最近傍距離の母集団から dist2 を選択するための閾値を設定
  * @return distance: 最近傍距離の母集団から dist2 を選択するための閾値
  */
  virtual void setDist2PercentileEstimate(float distance) = 0;

protected:
  EncoderAttributeParams() = default;
  virtual ~EncoderAttributeParams() = default;
  explicit EncoderAttributeParams(EncoderAttributeParams&&) noexcept = default;

private:
  EncoderAttributeParams(const EncoderAttributeParams&) = delete;
  EncoderAttributeParams& operator=(const EncoderAttributeParams&) = delete;
  EncoderAttributeParams&
  operator=(EncoderAttributeParams&&) noexcept = delete;
};

//----------------------------------------------------------------------------
/**
* @brief AttributeParameterSetVector定義
* @details AttributeParameterSetクラスのベクタ列としてAttributeParameterSetVectorを定義
*/
typedef ObjectVectorAdapter<AttributeParameterSet> AttributeParameterSetVector;

/**
* @brief EncoderAttributeParamsVector定義
* @details EncoderAttributeParamsクラスのベクタ列としてEncoderAttributeParamsVectorを定義
*/
typedef ObjectVectorAdapter<EncoderAttributeParams>
  EncoderAttributeParamsVector;

/**
* @brief EncoderParamsクラス
* @details エンコーダに関するパラメータを保持するクラス
*/
class GPCCLIB_INTERFACE_SPEC EncoderParams {
public:
  
  /**
  * @brief インスタンス生成
  * @details EncoderParamsクラスのインスタンスを生成する
  * @return EncoderParams* 生成したEncoderParamsインスタンスのポインタ
  */
  static EncoderParams* create();
  
  /**
  * @brief インスタンス解放
  * @details EncoderParamsクラスのインスタンスを解放する
  */
  virtual void release() = 0;

  /**
  * @brief  並列エンコードする際のスレッド数を取得
  * @details 並列エンコードする際のスレッド数を取得
  * @return 並列エンコードする際のスレッド数
  */
  virtual unsigned int getNumOfFrameThreads() const = 0;

  /**
  * @brief  並列エンコードする際のスレッド数を設定
  * @details 並列エンコードする際のスレッド数を設定
  * @param[in] numOfThreads: 並列エンコードする際のスレッド数
  */
  virtual void setNumOfFrameThreads(unsigned int numOfThreads) = 0;
  
  /**
  * @brief  並列エンコードの実行可否を取得
  * @details 並列エンコードの実行可否を取得
  * @return TRUE:並列エンコード実行可　FALSE:並列エンコード実行禁止
  */
  virtual bool doSliceParallelProc() const = 0;

  /**
  * @brief  並列エンコードの実行可否を設定
  * @details 並列エンコードの実行可否を設定
  * @param[in]  doSliceParallel: TRUE:並列エンコード実行可　FALSE:並列エンコード実行禁止
  */
  virtual void setDoSliceParallelProc(bool doSliceParallel) = 0;

  /**
  * @brief  方位角ソートの実行可否を取得
  * @details 方位角ソートの実行可否を取得
  * @return TRUE:方位角ソート実行可　FALSE:方位角ソート実行禁止
  */
  virtual bool shouldSortInputByAzimuth() const = 0;

  /**
  * @brief  方位角ソートの実行可否を設定
  * @details 方位角ソートの実行可否を設定
  * @param[in] enabled: TRUE:方位角ソート実行可　FALSE:方位角ソート実行禁止
  */
  virtual void setSortInputByAzimuth(bool enabled) = 0;
  
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
  
  /**
  * @note TODO.調査中(2)
  * @brief  SequenceParameterSetを取得
  * @details SequenceParameterSet(分割されたタイル間に関するパラメータセット)の実体を取得
  * @return SequenceParameterSetの実体
  */
  virtual const SequenceParameterSet& getSps() const = 0;
  
  /**
  * @brief  SequenceParameterSetを取得
  * @details SequenceParameterSet(分割されたタイル間に関するパラメータセット)の実体を取得
  * @return SequenceParameterSetの実体
  */
  virtual SequenceParameterSet& getSps() = 0;
  
  /**
  * @brief  GeometryParameterSetを取得
  * @details GeometryParameterSet(ジオメトリーに関するパラメータセット)の実体を取得
  * @return GeometryParameterSetの実体
  */
  virtual const GeometryParameterSet& getGps() const = 0;

  /**
  * @brief  GeometryParameterSetを取得
  * @details GeometryParameterSet(ジオメトリーに関するパラメータセット)の実体を取得
  * @return GeometryParameterSetの実体
  */
  virtual GeometryParameterSet& getGps() = 0;
  
  /**
  * @note TODO.調査中(2)
  * @brief  GeometryBrickHeaderを取得
  * @details GeometryBrickHeader(Slice毎のBrickに関するヘッダ)の実体を取得
  * @return GeometryBrickHeaderの実体
  */
  virtual const GeometryBrickHeader& getGbh() const = 0;
  
  /**
  * @brief  GeometryBrickHeaderを取得
  * @details GeometryBrickHeader(Slice毎のBrickに関するヘッダ)の実体を取得
  * @return GeometryBrickHeaderの実体
  */
  virtual GeometryBrickHeader& getGbh() = 0;

  // NB: information about attributes is split between the SPS and the APS.
  //  => The SPS enumerates the attributes, the APS controls coding params.
  /**
  * @brief  AttributeParameterSetVectorを取得
  * @details AttributeParameterSetVectorの実体を取得。注：属性に関する情報は、SPSとAPSに分割される。SPSは属性を列挙し、APSはコーディングパラメータを制御する
  * @return AttributeParameterSetVectorの実体
  */
  virtual const AttributeParameterSetVector& getAps() const = 0;

  /**
  * @brief  AttributeParameterSetVectorを取得
  * @details AttributeParameterSetVectorの実体を取得。注：属性に関する情報は、SPSとAPSに分割される。SPSは属性を列挙し、APSはコーディングパラメータを制御する
  * @return AttributeParameterSetVectorの実体
  */
  virtual AttributeParameterSetVector& getAps() = 0;

  // std::map<std::string, int> attributeIdxMap;
  
  /**
  * @brief  Attributeの要素数を取得
  * @details Attributeの要素数を取得
  * @return Attributeの要素数
  */
  virtual size_t getNumOfAttributes() const = 0;

  /**
  * @brief  Attributeの名前を取得
  * @details 指定位置のAttributeの名前を取得
  * @param[in] pos: Attributeの位置
  * @return Attributeの名前
  */
  virtual const char* getAttributeName(size_t pos) const = 0;

  /**
  * @brief  指定Attributeが存在するかを取得
  * @details 指定Attributeが存在するかを取得
  * @param[in] name: 指定Attribute名
  * @return TRUE:存在　FALSE:存在しない
  */
  virtual bool hasAttributeIndex(const char* name) const = 0;

  /**
  * @brief  指定Attributeの番号を取得
  * @details 指定Attributeの番号を取得
  * @param[in] name: 指定Attribute名
  * @return 指定Attributeの番号
  */
  virtual size_t getAttributeIndex(const char* name) const = 0;

  /**
  * @brief  指定Attributeの追加
  * @details 指定したindexに指定Attributeを追加
  * @param[in] name: 指定Attribute名(value)
  * @param[in] index: 指定番号(key)
  */
  virtual void setAttributeIndex(const char* name, size_t index) = 0;

  /**
  * @brief  Attributeの全削除
  * @details Attributeの全削除
  */
  virtual void clearAttributeIndex() = 0;

  // Encoder specific parameters for attributes
  /**
  * @brief  エンコーダの全Attribute取得
  * @details エンコーダの全Attribute(エンコーダ固有の属性パラメータ)取得
  * @return EncoderAttributeParamsVector& エンコーダ固有の属性パラメータ
  */
  virtual const EncoderAttributeParamsVector& getAttr() const = 0;
  
  /**
  * @brief  エンコーダの全Attribute取得
  * @details エンコーダの全Attribute(エンコーダ固有の属性パラメータ)取得
  * @return EncoderAttributeParamsVector& エンコーダ固有の属性パラメータ
  */
  virtual EncoderAttributeParamsVector& getAttr() = 0;

  // Determine the sequence bonuding box using the first input frame
  /**
  * @brief  自動的にバウンディングボックスを計算するか
  * @details 自動的にバウンディングボックスを計算するか。最初の入力フレームを用いてバウンディングボックスを決定する。しない場合はパラメータよりバウンディングボックスを決める
  * @return TRUE:存在　FALSE:存在しない
  */
  virtual bool isAutoSeqBbox() const = 0;

  /**
  * @brief  自動的にバウンディングボックスを計算するか設定
  * @details 自動的にバウンディングボックスを計算するか設定。最初の入力フレームを用いてバウンディングボックスを決定する
  * @param[in] isAuto TRUE:自動的にバウンディングボックスを計算する　FALSE:自動的にバウンディングボックスを計算しない
  */
  virtual void setAutoSeqBbox(bool isAuto) = 0;

  // Length of the source point cloud unit vectors.
  /**
  * @brief  ソース点群の単位ベクトルの長さ
  * @details ソース点群の単位ベクトルの長さ
  * @return ソース点群の単位ベクトルの長さ
  */
  virtual double getSrcUnitLength() const = 0;

  /**
  * @brief  ソース点群の単位ベクトルの長さ
  * @details ソース点群の単位ベクトルの長さ
  * @return ソース点群の単位ベクトルの長さ
  */
  virtual void setSrcUnitLength(double length) = 0;

  // Scale factor used to define the coordinate system used for coding.
  // This is the coordinate system where slicing is performed.
  //  P_cod = P_src * codedGeomScale
  /**
  * @brief  エンコードで使用するスケール係数を取得
  * @details エンコードに使用される座標系を定義するために使用されるスケール係数。これはスライスが実行される座標系である。
  * @details P_cod(エンコード直前の座標) = P_src(入力座標） * codedGeomScale
  * @return エンコードで使用するスケール係数
  */
  virtual double getCodedGeomScale() const = 0;
  
  /**
  * @brief  エンコードで使用するスケール係数を設定
  * @details エンコードに使用される座標系を定義するために使用されるスケール係数。これはスライスが実行される座標系である。
  * @details P_cod(エンコード直前の座標) = P_src(入力座標） * codedGeomScale
  * @param[in] scale: エンコードで使用するスケール係数
  */
  virtual void setCodedGeomScale(double scale) = 0;

  // Scale factor used to define the sequence coordinate system.
  //  P_seq = P_src * seqGeomScale
  /**
  * @note TODO.調査中(2)
  * @brief  シーケンス座標系を定義するために使用されるスケール係数を取得
  * @details シーケンス座標系を定義するために使用されるスケール係数
  * @details P_seq(シーケンス座標) = P_src(入力座標) * seqGeomScale
  * @return シーケンス座標系で使用するスケール係数
  */
  virtual double getSeqGeomScale() const = 0;

  /**
  * @brief  シーケンス座標系を定義するために使用されるスケール係数を設定
  * @details シーケンス座標系を定義するために使用されるスケール係数
  * @details P_seq(シーケンス座標) = P_src(入力座標) * seqGeomScale
  * @param[in] scale: シーケンス座標系で使用するスケール係数
  */
  virtual void setSeqGeomScale(double scale) = 0;

  // Scale factor used to define the external coordinate system.
  //  P_ext = P_src * extGeomScale
  /**
  * @note TODO.調査中(2)
  * @brief  外部座標系を定義するために使用されるスケール係数を取得
  * @details 外部座標系を定義するために使用されるスケール係数
  * @details P_ext(外部座標) = P_src(入力座標) * extGeomScale
  * @return 外部座標系で使用するスケール係数
  */
  virtual double getExtGeomScale() const = 0;

  /**
  * @brief  外部座標系を定義するために使用されるスケール係数を設定
  * @details 外部座標系を定義するために使用されるスケール係数
  * @details P_ext(外部座標) = P_src(入力座標) * extGeomScale
  * @param[in] scale: 外部座標系で使用するスケール係数設定
  */
  virtual void setExtGeomScale(double scale) = 0;

  // Number of fractional bits used in output position representation.
  /**
  * @brief  位置出力を表す際に用いるビット数を取得
  * @details 位置出力を表す際に用いるビット数
  * @return 位置出力を表す際に用いるビット数
  */
  virtual int getOutputFpBits() const = 0;
  
  /**
  * @brief  位置出力を表す際に用いるビット数を設定
  * @details 位置出力を表す際に用いるビット数
  * @param[in] fractionbits: 位置出力を表す際に用いるビット数
  */
  virtual void setOutputFpBits(int fractionbits) = 0;

  // Encoder specific parameters for geometry
  /**
  * @brief  エンコーダで定義するジオメトリ用パラメータを取得
  * @details エンコーダで定義するジオメトリ用パラメータ
  * @return OctreeEncOpts&　エンコーダで定義するジオメトリ用パラメータ
  */
  virtual const OctreeEncOpts& getGeom() const = 0;
  
  /**
  * @brief  エンコーダで定義するジオメトリ用パラメータを取得
  * @details エンコーダで定義するジオメトリ用パラメータ
  * @return OctreeEncOpts&　エンコーダで定義するジオメトリ用パラメータ
  */
  virtual OctreeEncOpts& getGeom() = 0;

  // Options for the predictive geometry coder
  /**
  * @brief  予測ジオメトリエンコードのオプションを取得
  * @details 予測ジオメトリエンコードのオプション
  * @return PredGeomEncOpts&　予測ジオメトリエンコードのオプション
  */
  virtual const PredGeomEncOpts& getPredGeom() const = 0;

  /**
  * @brief  予測ジオメトリエンコードのオプションを取得
  * @details 予測ジオメトリエンコードのオプションを取得
  * @return PredGeomEncOpts&　予測ジオメトリエンコードのオプション
  */
  virtual PredGeomEncOpts& getPredGeom() = 0;

  // Parameters that control partitioning
  /**
  * @brief  パーティショニングを制御するパラメータを取得
  * @details パーティショニングを制御するパラメータを取得
  * @return PartitionParams&　パーティショニングを制御するパラメータ
  */
  virtual const PartitionParams& getPartition() const = 0;

  /**
  * @brief  パーティショニングを制御するパラメータを取得
  * @details パーティショニングを制御するパラメータを取得
  * @return PartitionParams&　パーティショニングを制御するパラメータ
  */
  virtual PartitionParams& getPartition() = 0;

  // attribute recolouring parameters
  /**
  * @brief  再カラーリングに関するパラメータを取得
  * @details 再カラーリングに関するパラメータを取得。再カラーリングは点の追加/削除された場合に必要となる
  * @return RecolourParams&　再カラーリングに関するパラメータの構造体
  */
  virtual const RecolourParams& getRecolour() const = 0;
  
  /**
  * @brief  再カラーリングに関するパラメータを取得
  * @details 再カラーリングに関するパラメータを取得。再カラーリングは点の追加/削除された場合に必要となる
  * @return RecolourParams&　再カラーリングに関するパラメータの構造体
  */
  virtual RecolourParams& getRecolour() = 0;

  // number of expected lasers
  /**
  * @brief  予想されるレーザー数を取得
  * @details 予想されるレーザー数を取得
  * @return 予想されるレーザー数
  */
  virtual int getNumLasers() const = 0;

  /**
  * @brief  予想されるレーザー数を設定
  * @details 予想されるレーザー数を設定
  * @param[in] num: 予想されるレーザー数
  */
  virtual void setNumLasers(int num) = 0;

  // floating Lasers' theta (have to be converted to fixed point in gps)
  /**
  * @brief  レーザー角度θを取得
  * @details 角度モードで使用される各既知レーザーの仰角θ。
  * 各仰角はx-y平面に対するラジアン単位で表される。角度ゼロは水平なレーザーを表す。 
  * 正の角度は水平より高い仰角を表す。 負の角度は水平より下の仰角を表す。
  * 角度θはgpsで固定点に変換する必要がある
  * ソフトウェアは浮動小数点角度を18ビットの固定小数点表現に変換する
  * @return DoubleVectorAdapter& レーザー角度θのベクタ列を取得
  */
  virtual const DoubleVectorAdapter& getLasersTheta() const = 0;
  
  /**
  * @brief  レーザー角度θを取得
  * @details 角度モードで使用される各既知レーザーの仰角θ。
  * 各仰角はx-y平面に対するラジアン単位で表される。角度ゼロは水平なレーザーを表す。 
  * 正の角度は水平より高い仰角を表す。 負の角度は水平より下の仰角を表す。
  * 角度θはgpsで固定点に変換する必要がある
  * ソフトウェアは浮動小数点角度を18ビットの固定小数点表現に変換する
  * @return DoubleVectorAdapter& レーザー角度θのベクタ列を取得
  */
  virtual DoubleVectorAdapter& getLasersTheta() = 0;

  // floating Lasers' H (have to be converted to fixed point in gps)
  /**
  * @brief  レーザー平面Zを取得
  * @details レーザー平面z(gpsで固定点に変換する必要がある)を取得
  * @return DoubleVectorAdapter& レーザー平面Zのベクタ列を取得
  */
  virtual const DoubleVectorAdapter& getLasersZ() const = 0;

  /**
  * @brief  レーザー平面Zを取得
  * @details 角度モードで使用される各既知レーザーの垂直オフセット
  * 各オフセットは入力点群座標系のz軸に沿って表現され、LiDARヘッド位置に対する
  * 垂直オフセットに対応
  * ソフトウェアは浮動小数点オフセットを3ビットの固定小数点表現に変換します。
  * @return DoubleVectorAdapter& レーザー平面Zのベクタ列を取得
  */
  virtual DoubleVectorAdapter& getLasersZ() = 0;

  // per-slice trisoup node sizes
  /**
  * @brief  Trisoupコーデックのノードサイズ(Log2)を取得
  * @details TrisoupコーデックのSlice毎のノードサイズ(Log2)を取得
  * @return Int32VectorAdapter& TrisoupコーデックのSlice毎のノードサイズ(Log2)
  */
  virtual const Int32VectorAdapter& getTrisoupNodeSizesLog2() const = 0;

  /**
  * @brief  Trisoupコーデックのノードサイズ(Log2)を取得
  * @details TrisoupコーデックのSlice毎のノードサイズ(Log2)を取得
  * @return Int32VectorAdapter& TrisoupコーデックのSlice毎のノードサイズ(Log2)
  */
  virtual Int32VectorAdapter& getTrisoupNodeSizesLog2() = 0;

  // Enable enforcement of level limits (encoder will abort if exceeded)
  /**
  * @brief  sliceの点の数がリミットを超えているかチェックするかを取得
  * @details レベル毎の各sliceの点の数がリミットを超えているかチェックするかを取得
  * @return TRUE:チェックする　FALSE:チェックしない
  */
  virtual bool isEnforceLevelLimits() const = 0;

  /**
  * @brief  sliceの点の数がリミットを超えているかチェックするかを設定する
  * @details レベル毎の各sliceの点の数がリミットを超えているかチェックするかを設定する
  * @param[in] enable: TRUE:チェックする　FALSE:チェックしない
  */
  virtual void setEnforceLevelLimits(bool enable) = 0;

  // Qp used for IDCM quantisation (used to derive HLS values)
   // per-slice trisoup node sizes
  /**
  * @brief  IDCM定量化に使用されるQp（HLS値の導出に使用される）を取得
  * @details IDCM定量化に使用されるQp（HLS値の導出に使用される）を取得
  * @return IDCM定量化に使用されるQp（HLS値の導出に使用される）を取得
  */
  virtual int getIdcmQp() const = 0;

  /**
  * @brief  IDCM定量化に使用されるQp（HLS値の導出に使用される）を設定
  * @details IDCM定量化に使用されるQp（HLS値の導出に使用される）を設定
  * @param[in] qp: IDCM定量化に使用されるQp（HLS値の導出に使用される）
  */
  virtual void setIdcmQp(int qp) = 0;

protected:
  EncoderParams() = default;
  virtual ~EncoderParams() = default;
  explicit EncoderParams(EncoderParams&&) noexcept = default;

private:
  EncoderParams(const EncoderParams&) = delete;
  EncoderParams& operator=(const EncoderParams&) = delete;
  EncoderParams& operator=(EncoderParams&&) noexcept = delete;
};

} // namespace encoder
} // namespace gpcclib
