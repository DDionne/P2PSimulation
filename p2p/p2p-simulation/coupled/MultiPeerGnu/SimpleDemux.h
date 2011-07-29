/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Network
*  Modeled as a labeled transition system.
*
*  AUTHOR: Alan
*
*
*******************************************************************/
/*handle duplicate inclusion of this header*/
#ifndef __NETWORK_H
#define __NETWORK_H
#define VERBOSE false
/** include files **/
#include "atomic.h"  // class Atomic
#include "NetEvent.h"
#include "..\complexmessages.h" // static functions to manipulate "coded" messages in a consistent way across components
#include "..\complexmessages2.h"
#include <queue> //STL for queue template class
/** declarations **/
class SimpleDemux: public Atomic
{
public:


	SimpleDemux( const string &name = "Network" ) ;	 // Default constructor

	~SimpleDemux();					// Destructor

	virtual string className() const
	{return "Network";};



	//a queue of networks events as defined in NetEvent.h [just a struct], using the Standard Template Library
	//for now no real use in defining this...
	class EventQueue : public queue<netevent> {
	public :
		EventQueue(){

		}
	};

	protected:
		Model &initFunction()
		{
			return *this;
		};

		Model &externalFunction( const ExternalMessage & );

		Model &internalFunction( const InternalMessage & );

		Model &outputFunction( const InternalMessage & );

	private:
		const Port &inroute ;
	Port &route_out0 ;
	Port &route_out1 ;
	Port &route_out2 ;
	Port &route_out3 ;
	Port &route_out4 ;
	Port &route_out5 ;
	Port &route_out6 ;
	Port &route_out7 ;
	Port &route_out8 ;
	Port &route_out9 ;
	Port &route_out10 ;
	Port &route_out11 ;
	Port &route_out12 ;
	Port &route_out13 ;
	Port &route_out14 ;
	Port &route_out15 ;
	Port &route_out16 ;
	Port &route_out17 ;
	Port &route_out18 ;
	Port &route_out19 ;
	Port &route_out20 ;
	Port &route_out21 ;
	Port &route_out22 ;
	Port &route_out23 ;
	Port &route_out24 ;
	Port &route_out25 ;
	Port &route_out26 ;
	Port &route_out27 ;
	Port &route_out28 ;
	Port &route_out29 ;
	Port &route_out30 ;
	Port &route_out31 ;
	Port &route_out32 ;
	Port &route_out33 ;
	Port &route_out34 ;
	Port &route_out35 ;
	Port &route_out36 ;
	Port &route_out37 ;
	Port &route_out38 ;
	Port &route_out39 ;
	Port &route_out40 ;
	Port &route_out41 ;
	Port &route_out42 ;
	Port &route_out43 ;
	Port &route_out44 ;
	Port &route_out45 ;
	Port &route_out46 ;
	Port &route_out47 ;
	Port &route_out48 ;
	Port &route_out49 ;
	Port &route_out50 ;
	Port &route_out51 ;
	Port &route_out52 ;
	Port &route_out53 ;
	Port &route_out54 ;
	Port &route_out55 ;
	Port &route_out56 ;
	Port &route_out57 ;
	Port &route_out58 ;
	Port &route_out59 ;
	Port &route_out60 ;
	Port &route_out61 ;
	Port &route_out62 ;
	Port &route_out63 ;
	Port &route_out64 ;
	Port &route_out65 ;
	Port &route_out66 ;
	Port &route_out67 ;
	Port &route_out68 ;
	Port &route_out69 ;
	Port &route_out70 ;
	Port &route_out71 ;
	Port &route_out72 ;
	Port &route_out73 ;
	Port &route_out74 ;
	Port &route_out75 ;
	Port &route_out76 ;
	Port &route_out77 ;
	Port &route_out78 ;
	Port &route_out79 ;
	Port &route_out80 ;
	Port &route_out81 ;
	Port &route_out82 ;
	Port &route_out83 ;
	Port &route_out84 ;
	Port &route_out85 ;
	Port &route_out86 ;
	Port &route_out87 ;
	Port &route_out88 ;
	Port &route_out89 ;
	Port &route_out90 ;
	Port &route_out91 ;
	Port &route_out92 ;
	Port &route_out93 ;
	Port &route_out94 ;
	Port &route_out95 ;
	Port &route_out96 ;
	Port &route_out97 ;
	Port &route_out98 ;
	Port &route_out99 ;
	Port &route_out100 ;
	Port &route_out101 ;
	Port &route_out102 ;
	Port &route_out103 ;
	Port &route_out104 ;
	Port &route_out105 ;
	Port &route_out106 ;
	Port &route_out107 ;
	Port &route_out108 ;
	Port &route_out109 ;
	Port &route_out110 ;
	Port &route_out111 ;
	Port &route_out112 ;
	Port &route_out113 ;
	Port &route_out114 ;
	Port &route_out115 ;
	Port &route_out116 ;
	Port &route_out117 ;
	Port &route_out118 ;
	Port &route_out119 ;
	Port &route_out120 ;
	Port &route_out121 ;
	Port &route_out122 ;
	Port &route_out123 ;
	Port &route_out124 ;
	Port &route_out125 ;
	Port &route_out126 ;
	Port &route_out127 ;
	Port &route_out128 ;
	Port &route_out129 ;
	Port &route_out130 ;
	Port &route_out131 ;
	Port &route_out132 ;
	Port &route_out133 ;
	Port &route_out134 ;
	Port &route_out135 ;
	Port &route_out136 ;
	Port &route_out137 ;
	Port &route_out138 ;
	Port &route_out139 ;
	Port &route_out140 ;
	Port &route_out141 ;
	Port &route_out142 ;
	Port &route_out143 ;
	Port &route_out144 ;
	Port &route_out145 ;
	Port &route_out146 ;
	Port &route_out147 ;
	Port &route_out148 ;
	Port &route_out149 ;
	Port &route_out150 ;
	Port &route_out151 ;
	Port &route_out152 ;
	Port &route_out153 ;
	Port &route_out154 ;
	Port &route_out155 ;
	Port &route_out156 ;
	Port &route_out157 ;
	Port &route_out158 ;
	Port &route_out159 ;
	Port &route_out160 ;
	Port &route_out161 ;
	Port &route_out162 ;
	Port &route_out163 ;
	Port &route_out164 ;
	Port &route_out165 ;
	Port &route_out166 ;
	Port &route_out167 ;
	Port &route_out168 ;
	Port &route_out169 ;
	Port &route_out170 ;
	Port &route_out171 ;
	Port &route_out172 ;
	Port &route_out173 ;
	Port &route_out174 ;
	Port &route_out175 ;
	Port &route_out176 ;
	Port &route_out177 ;
	Port &route_out178 ;
	Port &route_out179 ;
	Port &route_out180 ;
	Port &route_out181 ;
	Port &route_out182 ;
	Port &route_out183 ;
	Port &route_out184 ;
	Port &route_out185 ;
	Port &route_out186 ;
	Port &route_out187 ;
	Port &route_out188 ;
	Port &route_out189 ;
	Port &route_out190 ;
	Port &route_out191 ;
	Port &route_out192 ;
	Port &route_out193 ;
	Port &route_out194 ;
	Port &route_out195 ;
	Port &route_out196 ;
	Port &route_out197 ;
	Port &route_out198 ;
	Port &route_out199 ;
	Port &route_out200 ;
	Port &route_out201 ;
	Port &route_out202 ;
	Port &route_out203 ;
	Port &route_out204 ;
	Port &route_out205 ;
	Port &route_out206 ;
	Port &route_out207 ;
	Port &route_out208 ;
	Port &route_out209 ;
	Port &route_out210 ;
	Port &route_out211 ;
	Port &route_out212 ;
	Port &route_out213 ;
	Port &route_out214 ;
	Port &route_out215 ;
	Port &route_out216 ;
	Port &route_out217 ;
	Port &route_out218 ;
	Port &route_out219 ;
	Port &route_out220 ;
	Port &route_out221 ;
	Port &route_out222 ;
	Port &route_out223 ;
	Port &route_out224 ;
	Port &route_out225 ;
	Port &route_out226 ;
	Port &route_out227 ;
	Port &route_out228 ;
	Port &route_out229 ;
	Port &route_out230 ;
	Port &route_out231 ;
	Port &route_out232 ;
	Port &route_out233 ;
	Port &route_out234 ;
	Port &route_out235 ;
	Port &route_out236 ;
	Port &route_out237 ;
	Port &route_out238 ;
	Port &route_out239 ;
	Port &route_out240 ;
	Port &route_out241 ;
	Port &route_out242 ;
	Port &route_out243 ;
	Port &route_out244 ;
	Port &route_out245 ;
	Port &route_out246 ;
	Port &route_out247 ;
	Port &route_out248 ;
	Port &route_out249 ;
	Port &route_out250 ;
	Port &route_out251 ;
	Port &route_out252 ;
	Port &route_out253 ;
	Port &route_out254 ;
	Port &route_out255 ;
	Port &route_out256 ;
	Port &route_out257 ;
	Port &route_out258 ;
	Port &route_out259 ;
	Port &route_out260 ;
	Port &route_out261 ;
	Port &route_out262 ;
	Port &route_out263 ;
	Port &route_out264 ;
	Port &route_out265 ;
	Port &route_out266 ;
	Port &route_out267 ;
	Port &route_out268 ;
	Port &route_out269 ;
	Port &route_out270 ;
	Port &route_out271 ;
	Port &route_out272 ;
	Port &route_out273 ;
	Port &route_out274 ;
	Port &route_out275 ;
	Port &route_out276 ;
	Port &route_out277 ;
	Port &route_out278 ;
	Port &route_out279 ;
	Port &route_out280 ;
	Port &route_out281 ;
	Port &route_out282 ;
	Port &route_out283 ;
	Port &route_out284 ;
	Port &route_out285 ;
	Port &route_out286 ;
	Port &route_out287 ;
	Port &route_out288 ;
	Port &route_out289 ;
	Port &route_out290 ;
	Port &route_out291 ;
	Port &route_out292 ;
	Port &route_out293 ;
	Port &route_out294 ;
	Port &route_out295 ;
	Port &route_out296 ;
	Port &route_out297 ;
	Port &route_out298 ;
	Port &route_out299 ;
	Port &route_out300 ;
	Port &route_out301 ;
	Port &route_out302 ;
	Port &route_out303 ;
	Port &route_out304 ;
	Port &route_out305 ;
	Port &route_out306 ;
	Port &route_out307 ;
	Port &route_out308 ;
	Port &route_out309 ;
	Port &route_out310 ;
	Port &route_out311 ;
	Port &route_out312 ;
	Port &route_out313 ;
	Port &route_out314 ;
	Port &route_out315 ;
	Port &route_out316 ;
	Port &route_out317 ;
	Port &route_out318 ;
	Port &route_out319 ;
	Port &route_out320 ;
	Port &route_out321 ;
	Port &route_out322 ;
	Port &route_out323 ;
	Port &route_out324 ;
	Port &route_out325 ;
	Port &route_out326 ;
	Port &route_out327 ;
	Port &route_out328 ;
	Port &route_out329 ;
	Port &route_out330 ;
	Port &route_out331 ;
	Port &route_out332 ;
	Port &route_out333 ;
	Port &route_out334 ;
	Port &route_out335 ;
	Port &route_out336 ;
	Port &route_out337 ;
	Port &route_out338 ;
	Port &route_out339 ;
	Port &route_out340 ;
	Port &route_out341 ;
	Port &route_out342 ;
	Port &route_out343 ;
	Port &route_out344 ;
	Port &route_out345 ;
	Port &route_out346 ;
	Port &route_out347 ;
	Port &route_out348 ;
	Port &route_out349 ;
	Port &route_out350 ;
	Port &route_out351 ;
	Port &route_out352 ;
	Port &route_out353 ;
	Port &route_out354 ;
	Port &route_out355 ;
	Port &route_out356 ;
	Port &route_out357 ;
	Port &route_out358 ;
	Port &route_out359 ;
	Port &route_out360 ;
	Port &route_out361 ;
	Port &route_out362 ;
	Port &route_out363 ;
	Port &route_out364 ;
	Port &route_out365 ;
	Port &route_out366 ;
	Port &route_out367 ;
	Port &route_out368 ;
	Port &route_out369 ;
	Port &route_out370 ;
	Port &route_out371 ;
	Port &route_out372 ;
	Port &route_out373 ;
	Port &route_out374 ;
	Port &route_out375 ;
	Port &route_out376 ;
	Port &route_out377 ;
	Port &route_out378 ;
	Port &route_out379 ;
	Port &route_out380 ;
	Port &route_out381 ;
	Port &route_out382 ;
	Port &route_out383 ;
	Port &route_out384 ;
	Port &route_out385 ;
	Port &route_out386 ;
	Port &route_out387 ;
	Port &route_out388 ;
	Port &route_out389 ;
	Port &route_out390 ;
	Port &route_out391 ;
	Port &route_out392 ;
	Port &route_out393 ;
	Port &route_out394 ;
	Port &route_out395 ;
	Port &route_out396 ;
	Port &route_out397 ;
	Port &route_out398 ;
	Port &route_out399 ;
	Port &route_out400 ;
	Port &route_out401 ;
	Port &route_out402 ;
	Port &route_out403 ;
	Port &route_out404 ;
	Port &route_out405 ;
	Port &route_out406 ;
	Port &route_out407 ;
	Port &route_out408 ;
	Port &route_out409 ;
	Port &route_out410 ;
	Port &route_out411 ;
	Port &route_out412 ;
	Port &route_out413 ;
	Port &route_out414 ;
	Port &route_out415 ;
	Port &route_out416 ;
	Port &route_out417 ;
	Port &route_out418 ;
	Port &route_out419 ;
	Port &route_out420 ;
	Port &route_out421 ;
	Port &route_out422 ;
	Port &route_out423 ;
	Port &route_out424 ;
	Port &route_out425 ;
	Port &route_out426 ;
	Port &route_out427 ;
	Port &route_out428 ;
	Port &route_out429 ;
	Port &route_out430 ;
	Port &route_out431 ;
	Port &route_out432 ;
	Port &route_out433 ;
	Port &route_out434 ;
	Port &route_out435 ;
	Port &route_out436 ;
	Port &route_out437 ;
	Port &route_out438 ;
	Port &route_out439 ;
	Port &route_out440 ;
	Port &route_out441 ;
	Port &route_out442 ;
	Port &route_out443 ;
	Port &route_out444 ;
	Port &route_out445 ;
	Port &route_out446 ;
	Port &route_out447 ;
	Port &route_out448 ;
	Port &route_out449 ;
	Port &route_out450 ;
	Port &route_out451 ;
	Port &route_out452 ;
	Port &route_out453 ;
	Port &route_out454 ;
	Port &route_out455 ;
	Port &route_out456 ;
	Port &route_out457 ;
	Port &route_out458 ;
	Port &route_out459 ;
	Port &route_out460 ;
	Port &route_out461 ;
	Port &route_out462 ;
	Port &route_out463 ;
	Port &route_out464 ;
	Port &route_out465 ;
	Port &route_out466 ;
	Port &route_out467 ;
	Port &route_out468 ;
	Port &route_out469 ;
	Port &route_out470 ;
	Port &route_out471 ;
	Port &route_out472 ;
	Port &route_out473 ;
	Port &route_out474 ;
	Port &route_out475 ;
	Port &route_out476 ;
	Port &route_out477 ;
	Port &route_out478 ;
	Port &route_out479 ;
	Port &route_out480 ;
	Port &route_out481 ;
	Port &route_out482 ;
	Port &route_out483 ;
	Port &route_out484 ;
	Port &route_out485 ;
	Port &route_out486 ;
	Port &route_out487 ;
	Port &route_out488 ;
	Port &route_out489 ;
	Port &route_out490 ;
	Port &route_out491 ;
	Port &route_out492 ;
	Port &route_out493 ;
	Port &route_out494 ;
	Port &route_out495 ;
	Port &route_out496 ;
	Port &route_out497 ;
	Port &route_out498 ;
	Port &route_out499 ;
	Port &route_out500 ;
	Port &route_out501 ;
	Port &route_out502 ;
	Port &route_out503 ;
	Port &route_out504 ;
	Port &route_out505 ;
	Port &route_out506 ;
	Port &route_out507 ;
	Port &route_out508 ;
	Port &route_out509 ;
	Port &route_out510 ;
	Port &route_out511 ;
	Port &route_out512 ;
	Port &route_out513 ;
	Port &route_out514 ;
	Port &route_out515 ;
	Port &route_out516 ;
	Port &route_out517 ;
	Port &route_out518 ;
	Port &route_out519 ;
	Port &route_out520 ;
	Port &route_out521 ;
	Port &route_out522 ;
	Port &route_out523 ;
	Port &route_out524 ;
	Port &route_out525 ;
	Port &route_out526 ;
	Port &route_out527 ;
	Port &route_out528 ;
	Port &route_out529 ;
	Port &route_out530 ;
	Port &route_out531 ;
	Port &route_out532 ;
	Port &route_out533 ;
	Port &route_out534 ;
	Port &route_out535 ;
	Port &route_out536 ;
	Port &route_out537 ;
	Port &route_out538 ;
	Port &route_out539 ;
	Port &route_out540 ;
	Port &route_out541 ;
	Port &route_out542 ;
	Port &route_out543 ;
	Port &route_out544 ;
	Port &route_out545 ;
	Port &route_out546 ;
	Port &route_out547 ;
	Port &route_out548 ;
	Port &route_out549 ;
	Port &route_out550 ;
	Port &route_out551 ;
	Port &route_out552 ;
	Port &route_out553 ;
	Port &route_out554 ;
	Port &route_out555 ;
	Port &route_out556 ;
	Port &route_out557 ;
	Port &route_out558 ;
	Port &route_out559 ;
	Port &route_out560 ;
	Port &route_out561 ;
	Port &route_out562 ;
	Port &route_out563 ;
	Port &route_out564 ;
	Port &route_out565 ;
	Port &route_out566 ;
	Port &route_out567 ;
	Port &route_out568 ;
	Port &route_out569 ;
	Port &route_out570 ;
	Port &route_out571 ;
	Port &route_out572 ;
	Port &route_out573 ;
	Port &route_out574 ;
	Port &route_out575 ;
	Port &route_out576 ;
	Port &route_out577 ;
	Port &route_out578 ;
	Port &route_out579 ;
	Port &route_out580 ;
	Port &route_out581 ;
	Port &route_out582 ;
	Port &route_out583 ;
	Port &route_out584 ;
	Port &route_out585 ;
	Port &route_out586 ;
	Port &route_out587 ;
	Port &route_out588 ;
	Port &route_out589 ;
	Port &route_out590 ;
	Port &route_out591 ;
	Port &route_out592 ;
	Port &route_out593 ;
	Port &route_out594 ;
	Port &route_out595 ;
	Port &route_out596 ;
	Port &route_out597 ;
	Port &route_out598 ;
	Port &route_out599 ;
	Port &route_out600 ;
	Port &route_out601 ;
	Port &route_out602 ;
	Port &route_out603 ;
	Port &route_out604 ;
	Port &route_out605 ;
	Port &route_out606 ;
	Port &route_out607 ;
	Port &route_out608 ;
	Port &route_out609 ;
	Port &route_out610 ;
	Port &route_out611 ;
	Port &route_out612 ;
	Port &route_out613 ;
	Port &route_out614 ;
	Port &route_out615 ;
	Port &route_out616 ;
	Port &route_out617 ;
	Port &route_out618 ;
	Port &route_out619 ;
	Port &route_out620 ;
	Port &route_out621 ;
	Port &route_out622 ;
	Port &route_out623 ;
	Port &route_out624 ;
	Port &route_out625 ;
	Port &route_out626 ;
	Port &route_out627 ;
	Port &route_out628 ;
	Port &route_out629 ;
	Port &route_out630 ;
	Port &route_out631 ;
	Port &route_out632 ;
	Port &route_out633 ;
	Port &route_out634 ;
	Port &route_out635 ;
	Port &route_out636 ;
	Port &route_out637 ;
	Port &route_out638 ;
	Port &route_out639 ;
	Port &route_out640 ;
	Port &route_out641 ;
	Port &route_out642 ;
	Port &route_out643 ;
	Port &route_out644 ;
	Port &route_out645 ;
	Port &route_out646 ;
	Port &route_out647 ;
	Port &route_out648 ;
	Port &route_out649 ;
	Port &route_out650 ;
	Port &route_out651 ;
	Port &route_out652 ;
	Port &route_out653 ;
	Port &route_out654 ;
	Port &route_out655 ;
	Port &route_out656 ;
	Port &route_out657 ;
	Port &route_out658 ;
	Port &route_out659 ;
	Port &route_out660 ;
	Port &route_out661 ;
	Port &route_out662 ;
	Port &route_out663 ;
	Port &route_out664 ;
	Port &route_out665 ;
	Port &route_out666 ;
	Port &route_out667 ;
	Port &route_out668 ;
	Port &route_out669 ;
	Port &route_out670 ;
	Port &route_out671 ;
	Port &route_out672 ;
	Port &route_out673 ;
	Port &route_out674 ;
	Port &route_out675 ;
	Port &route_out676 ;
	Port &route_out677 ;
	Port &route_out678 ;
	Port &route_out679 ;
	Port &route_out680 ;
	Port &route_out681 ;
	Port &route_out682 ;
	Port &route_out683 ;
	Port &route_out684 ;
	Port &route_out685 ;
	Port &route_out686 ;
	Port &route_out687 ;
	Port &route_out688 ;
	Port &route_out689 ;
	Port &route_out690 ;
	Port &route_out691 ;
	Port &route_out692 ;
	Port &route_out693 ;
	Port &route_out694 ;
	Port &route_out695 ;
	Port &route_out696 ;
	Port &route_out697 ;
	Port &route_out698 ;
	Port &route_out699 ;
	Port &route_out700 ;
	Port &route_out701 ;
	Port &route_out702 ;
	Port &route_out703 ;
	Port &route_out704 ;
	Port &route_out705 ;
	Port &route_out706 ;
	Port &route_out707 ;
	Port &route_out708 ;
	Port &route_out709 ;
	Port &route_out710 ;
	Port &route_out711 ;
	Port &route_out712 ;
	Port &route_out713 ;
	Port &route_out714 ;
	Port &route_out715 ;
	Port &route_out716 ;
	Port &route_out717 ;
	Port &route_out718 ;
	Port &route_out719 ;
	Port &route_out720 ;
	Port &route_out721 ;
	Port &route_out722 ;
	Port &route_out723 ;
	Port &route_out724 ;
	Port &route_out725 ;
	Port &route_out726 ;
	Port &route_out727 ;
	Port &route_out728 ;
	Port &route_out729 ;
	Port &route_out730 ;
	Port &route_out731 ;
	Port &route_out732 ;
	Port &route_out733 ;
	Port &route_out734 ;
	Port &route_out735 ;
	Port &route_out736 ;
	Port &route_out737 ;
	Port &route_out738 ;
	Port &route_out739 ;
	Port &route_out740 ;
	Port &route_out741 ;
	Port &route_out742 ;
	Port &route_out743 ;
	Port &route_out744 ;
	Port &route_out745 ;
	Port &route_out746 ;
	Port &route_out747 ;
	Port &route_out748 ;
	Port &route_out749 ;
	Port &route_out750 ;
	Port &route_out751 ;
	Port &route_out752 ;
	Port &route_out753 ;
	Port &route_out754 ;
	Port &route_out755 ;
	Port &route_out756 ;
	Port &route_out757 ;
	Port &route_out758 ;
	Port &route_out759 ;
	Port &route_out760 ;
	Port &route_out761 ;
	Port &route_out762 ;
	Port &route_out763 ;
	Port &route_out764 ;
	Port &route_out765 ;
	Port &route_out766 ;
	Port &route_out767 ;
	Port &route_out768 ;
	Port &route_out769 ;
	Port &route_out770 ;
	Port &route_out771 ;
	Port &route_out772 ;
	Port &route_out773 ;
	Port &route_out774 ;
	Port &route_out775 ;
	Port &route_out776 ;
	Port &route_out777 ;
	Port &route_out778 ;
	Port &route_out779 ;
	Port &route_out780 ;
	Port &route_out781 ;
	Port &route_out782 ;
	Port &route_out783 ;
	Port &route_out784 ;
	Port &route_out785 ;
	Port &route_out786 ;
	Port &route_out787 ;
	Port &route_out788 ;
	Port &route_out789 ;
	Port &route_out790 ;
	Port &route_out791 ;
	Port &route_out792 ;
	Port &route_out793 ;
	Port &route_out794 ;
	Port &route_out795 ;
	Port &route_out796 ;
	Port &route_out797 ;
	Port &route_out798 ;
	Port &route_out799 ;
	Port &route_out800 ;
	Port &route_out801 ;
	Port &route_out802 ;
	Port &route_out803 ;
	Port &route_out804 ;
	Port &route_out805 ;
	Port &route_out806 ;
	Port &route_out807 ;
	Port &route_out808 ;
	Port &route_out809 ;
	Port &route_out810 ;
	Port &route_out811 ;
	Port &route_out812 ;
	Port &route_out813 ;
	Port &route_out814 ;
	Port &route_out815 ;
	Port &route_out816 ;
	Port &route_out817 ;
	Port &route_out818 ;
	Port &route_out819 ;
	Port &route_out820 ;
	Port &route_out821 ;
	Port &route_out822 ;
	Port &route_out823 ;
	Port &route_out824 ;
	Port &route_out825 ;
	Port &route_out826 ;
	Port &route_out827 ;
	Port &route_out828 ;
	Port &route_out829 ;
	Port &route_out830 ;
	Port &route_out831 ;
	Port &route_out832 ;
	Port &route_out833 ;
	Port &route_out834 ;
	Port &route_out835 ;
	Port &route_out836 ;
	Port &route_out837 ;
	Port &route_out838 ;
	Port &route_out839 ;
	Port &route_out840 ;
	Port &route_out841 ;
	Port &route_out842 ;
	Port &route_out843 ;
	Port &route_out844 ;
	Port &route_out845 ;
	Port &route_out846 ;
	Port &route_out847 ;
	Port &route_out848 ;
	Port &route_out849 ;
	Port &route_out850 ;
	Port &route_out851 ;
	Port &route_out852 ;
	Port &route_out853 ;
	Port &route_out854 ;
	Port &route_out855 ;
	Port &route_out856 ;
	Port &route_out857 ;
	Port &route_out858 ;
	Port &route_out859 ;
	Port &route_out860 ;
	Port &route_out861 ;
	Port &route_out862 ;
	Port &route_out863 ;
	Port &route_out864 ;
	Port &route_out865 ;
	Port &route_out866 ;
	Port &route_out867 ;
	Port &route_out868 ;
	Port &route_out869 ;
	Port &route_out870 ;
	Port &route_out871 ;
	Port &route_out872 ;
	Port &route_out873 ;
	Port &route_out874 ;
	Port &route_out875 ;
	Port &route_out876 ;
	Port &route_out877 ;
	Port &route_out878 ;
	Port &route_out879 ;
	Port &route_out880 ;
	Port &route_out881 ;
	Port &route_out882 ;
	Port &route_out883 ;
	Port &route_out884 ;
	Port &route_out885 ;
	Port &route_out886 ;
	Port &route_out887 ;
	Port &route_out888 ;
	Port &route_out889 ;
	Port &route_out890 ;
	Port &route_out891 ;
	Port &route_out892 ;
	Port &route_out893 ;
	Port &route_out894 ;
	Port &route_out895 ;
	Port &route_out896 ;
	Port &route_out897 ;
	Port &route_out898 ;
	Port &route_out899 ;
	Port &route_out900 ;
	Port &route_out901 ;
	Port &route_out902 ;
	Port &route_out903 ;
	Port &route_out904 ;
	Port &route_out905 ;
	Port &route_out906 ;
	Port &route_out907 ;
	Port &route_out908 ;
	Port &route_out909 ;
	Port &route_out910 ;
	Port &route_out911 ;
	Port &route_out912 ;
	Port &route_out913 ;
	Port &route_out914 ;
	Port &route_out915 ;
	Port &route_out916 ;
	Port &route_out917 ;
	Port &route_out918 ;
	Port &route_out919 ;
	Port &route_out920 ;
	Port &route_out921 ;
	Port &route_out922 ;
	Port &route_out923 ;
	Port &route_out924 ;
	Port &route_out925 ;
	Port &route_out926 ;
	Port &route_out927 ;
	Port &route_out928 ;
	Port &route_out929 ;
	Port &route_out930 ;
	Port &route_out931 ;
	Port &route_out932 ;
	Port &route_out933 ;
	Port &route_out934 ;
	Port &route_out935 ;
	Port &route_out936 ;
	Port &route_out937 ;
	Port &route_out938 ;
	Port &route_out939 ;
	Port &route_out940 ;
	Port &route_out941 ;
	Port &route_out942 ;
	Port &route_out943 ;
	Port &route_out944 ;
	Port &route_out945 ;
	Port &route_out946 ;
	Port &route_out947 ;
	Port &route_out948 ;
	Port &route_out949 ;
	Port &route_out950 ;
	Port &route_out951 ;
	Port &route_out952 ;
	Port &route_out953 ;
	Port &route_out954 ;
	Port &route_out955 ;
	Port &route_out956 ;
	Port &route_out957 ;
	Port &route_out958 ;
	Port &route_out959 ;
	Port &route_out960 ;
	Port &route_out961 ;
	Port &route_out962 ;
	Port &route_out963 ;
	Port &route_out964 ;
	Port &route_out965 ;
	Port &route_out966 ;
	Port &route_out967 ;
	Port &route_out968 ;
	Port &route_out969 ;
	Port &route_out970 ;
	Port &route_out971 ;
	Port &route_out972 ;
	Port &route_out973 ;
	Port &route_out974 ;
	Port &route_out975 ;
	Port &route_out976 ;
	Port &route_out977 ;
	Port &route_out978 ;
	Port &route_out979 ;
	Port &route_out980 ;
	Port &route_out981 ;
	Port &route_out982 ;
	Port &route_out983 ;
	Port &route_out984 ;
	Port &route_out985 ;
	Port &route_out986 ;
	Port &route_out987 ;
	Port &route_out988 ;
	Port &route_out989 ;
	Port &route_out990 ;
	Port &route_out991 ;
	Port &route_out992 ;
	Port &route_out993 ;
	Port &route_out994 ;
	Port &route_out995 ;
	Port &route_out996 ;
	Port &route_out997 ;
	Port &route_out998 ;
	Port &route_out999 ;
EventQueue EvQ;
};	// class SimpleDemux


#endif    //__NETWORK_H
