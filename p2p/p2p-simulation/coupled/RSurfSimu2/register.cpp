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
#include "Router.h"              // class Gnutella
#include "RandomSurfer2.h"              //class RandomSurfer
#include "msgIdGen.h"                  //class msgIdGen
#include "PhysicalNetwork.h"               //class PhysicalNetwork
#include "NetworkGraph.h"                //class Network
#include "ConnectionManager.h"
#include "Logger.h"
#include "Repository.h"
#include "LinkFinder.h"


void MainSimulator::registerNewAtomics()
{
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<NetworkGraph>(), "NetworkGraph" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Router>() , "Router" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<RandomSurfer2>() , "RandomSurfer2" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<msgIdGen>() , "msgIdGen" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<PhysicalNetwork>(), "PhysicalNetwork") ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<ConnectionManager>(), "ConnectionManager") ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Logger>(), "Logger") ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Repository>(), "Repository") ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<LinkFinder>(), "LinkFinder") ;


};
