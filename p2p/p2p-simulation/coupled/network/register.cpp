/*******************************************************************
*
*  DESCRIPTION: Simulator::registerNewAtomics()
*
*  AUTHOR: Amir Barylko & Jorge Beyoglonian
*
*  EMAIL: mailto://amir@dc.uba.ar
*         mailto://jbeyoglo@dc.uba.ar
*
*  DATE: 27/6/1998
*
*******************************************************************/

#include "modeladm.h"
#include "mainsimu.h"
#include "LTSNetwork.h"              // class LTSNetwork
#include "Gnutella.h"
#include "database.h"
#include "demux.h"
#include "dispatcher.h"


void MainSimulator::registerNewAtomics()
{
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<LTSNetwork>() , "LTSNetwork" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Gnutella>() , "Gnutella" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Dispatcher>() , "Dispatcher" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Database>() , "Database" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Demux>() , "Demux" ) ;
};
