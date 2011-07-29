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
#include "PeerGnutella.h"              // class Gnutella
#include "RandomSurfer.h"              //class RandomSurfer
#include "msgIdGen.h"                  //class msgIdGen
#include "SimpleDemux.h"               //class SimpleDemux
#include "LTSNetwork.h"                //class Network
#include "ConnectionManager.h"
#include "Logger.h"
#include "Server.h"
#include "LinkFinder.h"


void MainSimulator::registerNewAtomics()
{
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<LTSNetwork>(), "LTSNetwork" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<PeerGnutella>() , "PeerGnutella" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<RandomSurfer>() , "RandomSurfer" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<msgIdGen>() , "msgIdGen" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<SimpleDemux>(), "SimpleDemux") ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<ConnectionManager>(), "ConnectionManager") ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Logger>(), "Logger") ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Server>(), "Server") ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<LinkFinder>(), "LinkFinder") ;


};
