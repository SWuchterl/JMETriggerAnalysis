#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <memory>

#include <TH1D.h>

class TrackHistogrammer : public edm::one::EDAnalyzer<edm::one::SharedResources> {

 public:
  explicit TrackHistogrammer(const edm::ParameterSet&);
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

 private:
  void beginJob() override {}
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override {}

  edm::EDGetToken tracks_token_;

  TH1D *h_track_mult_;
  TH1D *h_track_pt_;
  TH1D *h_track_eta_;
  TH1D *h_track_phi_;
  TH1D *h_track_outerPt_;
  TH1D *h_track_outerEta_;
  TH1D *h_track_outerPhi_;
};

TrackHistogrammer::TrackHistogrammer(const edm::ParameterSet& iConfig)
  : tracks_token_(consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("src"))){

  usesResource(TFileService::kSharedResource);

  edm::Service<TFileService> fs;

  if(not fs){

    throw edm::Exception(edm::errors::Configuration, "TFileService is not registered in cfg file");
  }

  h_track_mult_ = fs->make<TH1D>("track_mult", "track_mult", 240, 0, 12000);
  h_track_pt_ = fs->make<TH1D>("track_pt", "track_pt", 600, 0, 5.);
  h_track_eta_ = fs->make<TH1D>("track_eta", "track_eta", 600, -5., 5.);
  h_track_phi_ = fs->make<TH1D>("track_phi", "track_phi", 600, -3., 3.);

  h_track_outerPt_ = fs->make<TH1D>("track_outerPt", "track_outerPt", 600, 0, 5.);
  h_track_outerEta_ = fs->make<TH1D>("track_outerEta", "track_outerEta", 600, -5., 5.);
  h_track_outerPhi_ = fs->make<TH1D>("track_outerPhi", "track_outerPhi", 600, -3., 3.);
}

void TrackHistogrammer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByToken(tracks_token_, tracks);

  if(tracks.isValid()){

    h_track_mult_->Fill(tracks->size());

    for(const auto& trk : *tracks){

      h_track_pt_->Fill(trk.pt());
      h_track_eta_->Fill(trk.eta());
      h_track_phi_->Fill(trk.phi());

      h_track_outerPt_->Fill(trk.outerPt());
      h_track_outerEta_->Fill(trk.outerEta());
      h_track_outerPhi_->Fill(trk.outerPhi());
    }
  }
  else {

    edm::LogWarning("Input") << "invalid TrackCollection";
  }
}

void TrackHistogrammer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {

  edm::ParameterSetDescription desc;
//  desc.setUnknown();
  desc.add<edm::InputTag>("src")->setComment("edm::InputTag of input reco::TrackCollection");
  descriptions.add("TrackHistogrammer", desc);
}

DEFINE_FWK_MODULE(TrackHistogrammer);
