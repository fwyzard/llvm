// RUN: %check_clang_tidy %s cms-esrget %t

namespace std {
  class string {
  public:
    string() {};
    string(char * m): m_t(m) {}
    char * m_t;
  };
}

namespace edm {
  template <typename ESProduct, typename ESRecord>
  class ESGetToken {
  };


  template <typename T>
  class ESHandle {
  public:
    ESHandle() {};
    ESHandle(T const* m): m_t(m) {}
    T* m_t;
    T const& operator*() {return *m_t;};
    T const* operator->() const {return m_t;};
    explicit operator bool () const {return not( m_t == nullptr);};
    bool operator!() const { return m_t == nullptr;};
  };

  class EventSetupRecord {
  public:
    template <typename T>
    bool get(ESHandle<T>&);
    
    template <typename T>
    bool get(std::string const&, ESHandle<T>&);

  };

  template <typename T>
  class EventSetupRecordImplementation : public EventSetupRecord {
  public:

    using EventSetupRecord::get;

    template <typename PRODUCT>
    PRODUCT const& get(ESGetToken<PRODUCT, T> const & iToken) const;


    template <typename PRODUCT>
    PRODUCT const& get(ESGetToken<PRODUCT, T>& iToken) const;

  }; 
  
  class EventSetup {
  public:
    template <typename Rec>
    Rec get() { return Rec{}; }

    template <typename T, typename R>
    ESHandle<T> getHandle(const ESGetToken<T, R>& iToken) const {
      return ESHandle<T>{};
    };
  };
}

struct FooR : public edm::EventSetupRecord {};
struct FooP {};

class ESFooR : public edm::EventSetupRecordImplementation<ESFooR> {};


class Bar {
public:
  bool doWork(edm::EventSetup& iSetup, edm::ESGetToken<FooP, FooR> const& token);
  edm::ESGetToken<FooP, FooR> m_token;
  edm::ESHandle<FooP> m_handle;
};

bool Bar::doWork(edm::EventSetup& iSetup, edm::ESGetToken<FooP, FooR> const& token) {
  // this should give warning
  edm::ESHandle<FooP> handle;
  iSetup.get<FooR>().get(handle);
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit [cms-esrget]
// CHECK-FIXES: {{^}}  iSetup.get<FooR>().get(handle);{{$}}
  iSetup.get<FooR>().get("", handle);
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit [cms-esrget]

  // this would be the replacement
  handle = iSetup.getHandle(token);

  edm::EventSetupRecord ESR;
  ESR.get( handle );
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit [cms-esrget]
  ESR.get("test", handle);
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit [cms-esrget]
  
  edm::EventSetupRecord * PESR = &ESR;
  PESR->get( handle );
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit [cms-esrget]
  PESR->get("test", handle);
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit [cms-esrget]

  FooR fooR;
  fooR.get( handle );
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit [cms-esrget]
  fooR.get("test", handle);
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit [cms-esrget]
  
  edm::EventSetupRecord * pFooR = &fooR;
  pFooR->get( handle );
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit [cms-esrget]
  pFooR->get("test", handle);
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: direct call of function EventSetupRecord::get(ESHandle&) is deprecated and should be replaced with a call to EventSetup::getHandle(ESGetToken&). To use ESGetToken see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#In_ED_module To get data with the token see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHowToGetDataFromES#Getting_data_from_EventSetup_wit [cms-esrget]

  return true;
}

class Baz {
  public:
    edm::ESGetToken<FooR, ESFooR> m_token;
    bool doWork( ESFooR const& record);
};

bool Baz::doWork( ESFooR const& record) {

  const auto& esFooR = record.get(m_token);

  return true;
}
