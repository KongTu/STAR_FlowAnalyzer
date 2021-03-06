#ifndef STFLOWTREEMAKER_HH
#define STFLOWTREEMAKER_HH

/***************************************************************************
 *
 * $Id: StFlowTreeMaker.h 2015/04/09  Exp $ 
 * StFlowTreeMaker - class to produce miniTree for mtd related analysis
 * Author: Shuai Yang
 *--------------------------------------------------------------------------
 *
 ***************************************************************************/

#include "StMaker.h"
#include "StTreeStructure.h"

#include "StThreeVectorF.hh"
#include "TLorentzVector.h"
#include "TComplex.h"

#include "StMtdUtil/StMtdConstants.h"

#include <vector>
#include <map>
#ifndef ST_NO_NAMESPACES
using std::vector;
#endif

class TH1D;
class TH2D;
class TString;
class TTree;
class TFile;

class StMuDstMaker;
class StMuDst;
class StEmcCollection;
class StEmcPosition;
class StEmcGeom;
class StEmcRawHit;
class StMuTrack;
class StMuMtdHit;

class StFmsDbMaker;

class StPicoDstMaker;
class StPicoDst;
class StPicoTrack;
class StPicoMtdHit;

#if !defined(ST_NO_TEMPLATE_DEF_ARGS) || defined(__CINT__)
typedef vector<Int_t> IntVec;
typedef vector<Double_t> DoubleVec;
typedef vector<TLorentzVector> LorentzVec;
#else
typedef vector<Int_t, allocator<Int_t>> IntVec;
typedef vector<Double_t, allocator<Double_t>> DoubleVec;
typedef vector<TLorentzVector, allocator<TLorentzVector>> LorentzVec;
#endif

const Int_t          NPCAbin=10;

class StFlowTreeMaker : public StMaker {
	public:
		StFlowTreeMaker(const Char_t *name = "StFlowTreeMaker");
		~StFlowTreeMaker();

		Int_t    Init();
		Int_t    InitRun(const Int_t runNumber);
		Int_t    Make();
		Int_t    Finish();

		void     setVnHarmonics(const Double_t n_);
		void     setMaxVtxR(const Double_t max);
		void     setMaxVtxZ(const Double_t max);
		void     setMaxVzDiff(const Double_t max);
		void     setMinTrackPt(const Double_t min);
		void     setMaxTrackEta(const Double_t max);
		void     setMinNHitsFit(const Int_t min);
		void     setMinNHitsFitRatio(const Double_t min);
		void     setMinNHitsDedx(const Int_t min);
		void     setMaxDca(const Double_t max);
		void     setMaxnSigmaE(const Double_t max);
		void     setMaxBeta2TOF(const Double_t max);
		void     setFillHisto(const Bool_t fill);
		void     setFillTree(const Bool_t fill);
		void     setOutFileName(const TString name);
		void     setStreamName(const TString name);
		void     setPrintMemory(const Bool_t pMem);
		void     setPrintCpu(const Bool_t pCpu);
		void     setPrintConfig(const Bool_t print);

	protected:
		void     printConfig();
		void     bookTree();
		void     bookHistos();
		Bool_t   processPicoEvent();
		void     fillEventPlane();
		Bool_t   isValidTrack(StPicoTrack *pTrack, StThreeVectorF vtxPos) const;
		TComplex q_vector(double n, double p, double w, double phi);

		void     initEmc();
		void     finishEmc();
		void     buildEmcIndex();

		bool     getBEMC(const StMuTrack* t, int* id, int* adc, float* ene, float* d, int* nep, int* towid);
		//Bool_t   getBemcInfo(StMuTrack *pMuTrack, const Short_t nTrks, Short_t &nEMCTrks, Bool_t flag=kFALSE);
		Float_t getBemcInfo(StMuTrack *pMuTrack);
		//Bool_t   isMtdHitFiredTrigger(const StPicoMtdHit *hit);
		//Bool_t   isMtdHitFiredTrigger(const StMuMtdHit *hit);
		//Bool_t   isQTFiredTrigger(const Int_t qt, const Int_t pos);

		//void     triggerData();


	private:
		//variable for tree
		Int_t    mRunId;
		Int_t    mEventId;
		Int_t    mNTrigs;
		Int_t    mTrigId[64];
		Short_t  mRefMult;
		Short_t  mGRefMult;

		Float_t  mBBCRate;
		Float_t  mZDCRate;
		Float_t  mBField;
		Float_t  mVpdVz;
		Float_t  mVertexX;
		Float_t  mVertexY;
		Float_t  mVertexZ;


		//BBC
		Int_t    mBbcQ[48];

		//nTOF
		Int_t    mNTofHits;

		//ZDC
		Int_t mZDCwest;
		Int_t mZDCeast;

		//EEMC
		int mNEmc;
		float mEmcE[10000];
		float mEmcEta[10000];
		float mEmcPhi[10000];

		//track information
		Int_t  mNTrks;
		Int_t   mCharge[mMax];
		Float_t  mPt[mMax];
		Float_t  mEta[mMax];
		Float_t  mPhi[mMax];

		Float_t  mgPt[mMax];
		Float_t  mgEta[mMax];
		Float_t  mgPhi[mMax];

		Int_t mNHitsFit[mMax];
		Int_t mNHitsPoss[mMax];

		Float_t mDedx[mMax];
		Float_t mDndx[mMax];
		Float_t mNSigmaE[mMax];
		Float_t mDca[mMax];

		Float_t  mTOFLocalY[mMax];
		Float_t  mBeta2TOF[mMax];
		Float_t  mBEMCE[mMax];
		Float_t  mBEMCE0[mMax];
		Float_t  mBEMCE1[mMax];

		StMuDstMaker   *mMuDstMaker;          // Pointer to StMuDstMaker
		StMuDst        *mMuDst;              // Pointer to MuDst event
		StEmcPosition  *mEmcPosition;
		StEmcGeom      *mEmcGeom[4];
		StEmcRawHit*     mEmcIndex[4800];

		StPicoDstMaker *mPicoDstMaker;
		StPicoDst      *mPicoDst;
		StFmsDbMaker *fmsDbMaker;

		//StRefMultCorr *refMultCorr; //decide centrality

		Bool_t         mFillTree;            // Flag of fill the event tree
		Bool_t         mFillHisto;           // Flag of fill the histogram
		Bool_t         mPrintConfig;         // Flag to print out task configuration
		Bool_t         mPrintMemory;         // Flag to print out memory usage
		Bool_t         mPrintCpu;            // Flag to print out CPU usage
		TString        mStreamName;          // Data stream name
		TFile          *fOutFile;            // Output file
		TString        mOutFileName;         // Name of the output file 
		StEvtData      mEvtData;
		TTree          *mEvtTree;            // Pointer to the event tree

		Int_t		   mVn; 				 // vn harmonic order
		Double_t       mMaxVtxR;             // Maximum vertex r
		Double_t       mMaxVtxZ;             // Maximum vertex z
		Double_t       mMaxVzDiff;           // Maximum VpdVz-TpcVz
		Double_t       mMinTrkPt;            // Minimum track pt
		Double_t       mMaxTrkEta;           // Maximum track eta
		Int_t          mMinNHitsFit;         // Minimum number of hits used for track fit
		Double_t       mMinNHitsFitRatio;    // Minimum ratio of hits used for track fit
		Int_t          mMinNHitsDedx;        // Minimum number of hits used for de/dx
		Double_t       mMaxDca;              // Maximum track dca
		Double_t       mMaxnSigmaE;          // Maximum nSigmaE cut
		Double_t       mMaxBeta2TOF;         // Maximum |1-1./beta| for TpcE

		StEmcCollection *mEmcCollection;

		//IntVec         mStHLT_TriggerIDs;
		//IntVec         mStMTD_TriggerIDs;
		IntVec         mStPhysics_TriggerIDs;

		/*
		static const Int_t    mNQTs=8;        
		Int_t          mModuleToQT[30][5];            // Map from module to QT board index
		Int_t          mModuleToQTPos[30][5];         // Map from module to the position on QA board
		Int_t          mQTtoModule[mNQTs][8];         // Map from QT board to module index
		Int_t          mQTSlewBinEdge[mNQTs][16][8];  // Bin edges for online slewing correction for QT
		Int_t          mQTSlewCorr[mNQTs][16][8];         // Slewing correction values for QT
		Int_t          mTrigQTpos[mNQTs][2];              // corresponding QT position of fire trigger bit
		*/
		//define histograms ongoing...
		TH1D           *hEvent;
		TH1D		   *hVtxZ;
		TH1D		   *hRefMult;
		TH2D           *hVtxYvsVtxX;
		TH2D           *hVPDVzvsTPCVz;
		TH1D           *hVzDiff;
		TH2D           *hGRefMultvsGRefMultCorr;
		TH1D           *hCentrality;

		//Qvector histograms for PCA


		TH2D           *hdEdxvsP;
		TH2D           *hdNdxvsP;
		TH2D           *hnSigEvsP;
		TH2D           *hBetavsP;
		TH2D 		   *hFmsXYdis;

		TH1D* cn_tracker[20][2];
		TH1D* cn_tracker_fms[10][20][2];
		TH1D* cn_tracker_fms_real[10][20][2];
		TH1D* cn_tracker_fms_imag[10][20][2];

		TH1D* cn_QbQc[10];
		TH1D* cn_QaQb[10];
		TH1D* cn_QaQc[10];
		TH1D* cn_Qa_real[10];
		TH1D* cn_Qb_real[10];
		TH1D* cn_Qc_real[10];
		TH1D* cn_Qa_imag[10];
		TH1D* cn_Qb_imag[10];
		TH1D* cn_Qc_imag[10];

	

		ClassDef(StFlowTreeMaker, 1)
};
inline void StFlowTreeMaker::setVnHarmonics(const Double_t n_) { mVn = n_; }
inline void StFlowTreeMaker::setMaxVtxR(const Double_t max) { mMaxVtxR = max; }
inline void StFlowTreeMaker::setMaxVtxZ(const Double_t max) { mMaxVtxZ = max; }
inline void StFlowTreeMaker::setMaxVzDiff(const Double_t max) { mMaxVzDiff = max; }
inline void StFlowTreeMaker::setMinTrackPt(const Double_t min){ mMinTrkPt = min;}
inline void StFlowTreeMaker::setMaxTrackEta(const Double_t max){ mMaxTrkEta = max; }
inline void StFlowTreeMaker::setMinNHitsFit(const Int_t min) { mMinNHitsFit = min; }
inline void StFlowTreeMaker::setMinNHitsFitRatio(const Double_t min) { mMinNHitsFitRatio = min; }
inline void StFlowTreeMaker::setMinNHitsDedx(const Int_t min) { mMinNHitsDedx = min; }
inline void StFlowTreeMaker::setMaxDca(const Double_t max) { mMaxDca = max; }
inline void StFlowTreeMaker::setMaxnSigmaE(const Double_t max) { mMaxnSigmaE = max; }
inline void StFlowTreeMaker::setMaxBeta2TOF(const Double_t max) { mMaxBeta2TOF = max; }
inline void StFlowTreeMaker::setFillHisto(const Bool_t fill) { mFillHisto = fill; }
inline void StFlowTreeMaker::setFillTree(const Bool_t fill) { mFillTree = fill; }
inline void StFlowTreeMaker::setOutFileName(const TString name) { mOutFileName = name; }
inline void StFlowTreeMaker::setStreamName(const TString name) { mStreamName = name; }
inline void StFlowTreeMaker::setPrintMemory(const Bool_t pMem) { mPrintMemory = pMem; }
inline void StFlowTreeMaker::setPrintCpu(const Bool_t pCpu) { mPrintCpu = pCpu; }
inline void StFlowTreeMaker::setPrintConfig(const Bool_t print) { mPrintConfig = print; }
#endif
