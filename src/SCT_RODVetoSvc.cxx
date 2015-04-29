/**
 * @file SCT_RODVetoSvc.cxx
 * implementation file for service allowing one to declare modules as bad
 * @author daiki.hayakawa@cern.ch
**/

#include "SCT_RODVetoSvc.h"
//STL includes
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>

//Gaudi includes
#include "GaudiKernel/StatusCode.h"

#include "Identifier/IdentifierHash.h"

#include "InDetIdentifier/SCT_ID.h"

#include "SCT_ConditionsServices/ISCT_ConfigurationConditionsSvc.h"


template <class T> 
static std::vector<T> 
string2Vector(const std::string & s){
		std::vector<T> v;
		std::istringstream inputStream(s);
		std::istream_iterator<T> vecRead(inputStream);
		std::istream_iterator<T> endOfString; //relies on default constructor to produce eof
		std::copy(vecRead,endOfString,std::back_inserter(v));// DOESN'T ALLOW NON-WHITESPACE DELIMITER !
		return v;
	}

// Constructor
SCT_RODVetoSvc::SCT_RODVetoSvc( const std::string& name, ISvcLocator* pSvcLocator ) : 
  AthService(name, pSvcLocator), 
  m_cabling("SCT_CablingSvc",name),
  //m_badElements(0),
  m_filled(false), 
  m_pHelper(0),
  m_detStore("DetectorStore", name){
  declareProperty("BadRODIdentifiers",m_badRODElements);
}

//Initialize
StatusCode 
SCT_RODVetoSvc::initialize(){
  ATH_CHECK(m_detStore.retrieve());
  ATH_CHECK(m_detStore->retrieve(m_pHelper, "SCT_ID"));
  ATH_CHECK(m_cabling.retrieve());
  return  StatusCode::SUCCESS;
}

//Finalize
StatusCode
SCT_RODVetoSvc::finalize(){
  StatusCode sc(StatusCode::SUCCESS);
  //Code
  return sc;
}

// From s.binet
// Query the interfaces.
//   Input: riid, Requested interface ID
//          ppvInterface, Pointer to requested interface
//   Return: StatusCode indicating SUCCESS or FAILURE.
// N.B. Don't forget to release the interface after use!!!
StatusCode 
SCT_RODVetoSvc::queryInterface(const InterfaceID& riid, void** ppvInterface) 
{
  if ( ISCT_ConditionsSvc::interfaceID().versionMatch(riid) ) {
    *ppvInterface = dynamic_cast<ISCT_ConditionsSvc*>(this);
  } else {
    // Interface is not directly available : try out a base class
    return AthService::queryInterface(riid, ppvInterface);
  }
  addRef();
  return StatusCode::SUCCESS;
}

bool 
SCT_RODVetoSvc::canReportAbout(InDetConditions::Hierarchy h){
  return ((h==InDetConditions::DEFAULT) or (h==InDetConditions::SCT_SIDE));
}

bool 
SCT_RODVetoSvc::isGood(const Identifier & elementId, InDetConditions::Hierarchy h){
  if (not canReportAbout(h)) return true;
  if (not filled() and fillData().isFailure()) ATH_MSG_WARNING("Data structure could not be filled");
  bool result = (m_badIds.find(elementId) == m_badIds.end());
  return result;
}

bool 
SCT_RODVetoSvc::isGood(const IdentifierHash & hashId){
  if (not filled() and fillData().isFailure()) ATH_MSG_WARNING("Data structure could not be filled");
  Identifier elementId=m_pHelper->wafer_id(hashId);
  return isGood(elementId);
}

StatusCode 
SCT_RODVetoSvc::fillData(){
  if (m_badRODElements.value().empty()){
    ATH_MSG_INFO("No bad RODs in job options.");
    return StatusCode::SUCCESS;
  }
  ATH_MSG_INFO(m_badRODElements.value().size() <<" RODs were declared bad");
  bool success(true);
  for(int thisRod: m_badRODElements.value()){
    std::vector<IdentifierHash> listOfHashes;
    m_cabling->getHashesForRod(listOfHashes,thisRod);
    ATH_MSG_INFO("This rod is "<<thisRod);
    //Two consecutive hashes may produce the same module id, since they will be two sides
    //of the same module. We avoid invalid inserts by guarding against this.
    Identifier previousId; //constructor produces an invalid one
    for (IdentifierHash thisHash:listOfHashes){
      Identifier wafId = m_pHelper->wafer_id(thisHash);
      Identifier modId = m_pHelper->module_id(wafId);
      const bool alreadyInserted(modId==previousId);
      previousId=modId;
      if (alreadyInserted) continue;
      ATH_MSG_VERBOSE("This Id is "<<modId);
      const bool thisInsertionOk=m_badIds.insert(modId).second;
      if (not thisInsertionOk){
      	ATH_MSG_WARNING("Insertion failed for rod "<<thisRod<<" and module (hash,id): "<<thisHash<<", "<<modId);
      }
      success &= thisInsertionOk;
    }
    if(m_badIds.size() == 0) {
      ATH_MSG_ERROR("No module correspond to rod " << thisRod );
      return StatusCode::FAILURE;
    }
  }
  m_filled=success;
  if (m_filled) ATH_MSG_INFO("Structure successfully filled with "<<m_badIds.size()<<" modules.");
  return success?(StatusCode::SUCCESS):(StatusCode::FAILURE);
}

StatusCode 
SCT_RODVetoSvc::fillData(int& /*i*/ , std::list<std::string>& /*folderList*/){
	ATH_MSG_WARNING("A database fill callback was triggered for the SCT_RODVetoSvc, which does not use the database");
	return StatusCode::RECOVERABLE;
}

bool 
SCT_RODVetoSvc::canFillDuringInitialize(){
  return false;//uses the cabling, so cannot be filled in initialize
}
bool
SCT_RODVetoSvc::filled() const{
  //code
  return m_filled;
}

