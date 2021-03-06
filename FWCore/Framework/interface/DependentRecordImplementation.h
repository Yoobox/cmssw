#ifndef FWCore_Framework_DependentRecordImplementation_h
#define FWCore_Framework_DependentRecordImplementation_h
// -*- C++ -*-
//
// Package:     Framework
// Class  :     DependentRecordImplementation
// 
/**\class DependentRecordImplementation DependentRecordImplementation.h FWCore/Framework/interface/DependentRecordImplementation.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris Jones
// Created:     Fri Apr 29 10:03:54 EDT 2005
//

// system include files
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/find.hpp"
#include <sstream>
#include <type_traits>

// user include files
#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/NoRecordException.h"
#include "FWCore/Framework/interface/DependentRecordTag.h"

// forward declarations
namespace edm {
namespace eventsetup {
   
template< class RecordT, class ListT>
class DependentRecordImplementation : public EventSetupRecordImplementation<RecordT>, public DependentRecordTag
{

   public:
      DependentRecordImplementation() {}
      typedef ListT list_type;
      //virtual ~DependentRecordImplementation();
      
      // ---------- const member functions ---------------------
      template<class DepRecordT>
      const DepRecordT& getRecord() const {
        //Make sure that DepRecordT is a type in ListT
        typedef typename boost::mpl::end< ListT >::type EndItrT;
        typedef typename boost::mpl::find< ListT, DepRecordT>::type FoundItrT;
        static_assert(! std::is_same<FoundItrT, EndItrT>::value, "Trying to get a Record from another Record where the second Record is not dependent on the first Record.");
        try {
          EventSetup const& eventSetupT = this->eventSetup();
          return eventSetupT.get<DepRecordT>();
        } catch(cms::Exception& e) {
          std::ostringstream sstrm;
          sstrm <<"While getting dependent Record from Record "<<this->key().type().name();
          e.addContext(sstrm.str());
          throw;
        }
      }

      template<class DepRecordT>
      const DepRecordT* tryToGetRecord() const {
        //Make sure that DepRecordT is a type in ListT
        typedef typename boost::mpl::end< ListT >::type EndItrT;
        typedef typename boost::mpl::find< ListT, DepRecordT>::type FoundItrT;
        static_assert(! std::is_same<FoundItrT, EndItrT>::value, "Trying to get a Record from another Record where the second Record is not dependent on the first Record.");
        EventSetup const& eventSetupT = this->eventSetup();
        return eventSetupT.tryToGet<DepRecordT>();
      }

      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------

   private:
      DependentRecordImplementation(const DependentRecordImplementation&); // stop default

      const DependentRecordImplementation& operator=(const DependentRecordImplementation&); // stop default

      // ---------- member data --------------------------------

};

  }
}

#endif
