#include <gui/sweldingprocess_screen/SWeldingProcessView.hpp>
#include <gui/custom/CWeldingContainerMig.hpp>
#include <gui/custom/CWeldingContainerMma.hpp>
#include <gui/custom/CWeldingContainerTig.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>



SWeldingProcessView::SWeldingProcessView():PtrQuadro(NULL),setMenuSelectedContainer_Callback(this,&SWeldingProcessView::menucSelectedeContainer_Callback),process(0),focusMenu(0),encR(0),encL(0),indexMenu(0)
{

	cInfoBar.SetTitolo(T_TITLE_WELDING_PROCESS);
}

SWeldingProcessView::~SWeldingProcessView()
{
	if(PtrQuadro)
		_DELETE_(PtrQuadro);

}

void SWeldingProcessView::setupScreen()
{
   SWeldingProcessViewBase::setupScreen();
	process = presenter->getProcess();
   if ( PtrQuadro )
   {
      remove(*PtrQuadro);
      _DELETE_(PtrQuadro);
   }

   if ( process >= Model::MODEL_PROCESSO_MAX )
      process = VIPERDEF_PROCESSO_MIG;
   else if ( process < VIPERDEF_PROCESSO_MMA )
      process = VIPERDEF_PROCESSO_MMA;

   createSelectedProcess(Model::Process_Type (process));
}





void SWeldingProcessView::tearDownScreen()
{
    SWeldingProcessViewBase::tearDownScreen();
}

void SWeldingProcessView::GoBack()
{
   application().VisualizzaSMenuPrincipale();
}


void SWeldingProcessView::encRconfirm()
{
   presenter->setProcessToModel((Model::Process_Type)process);
}


void SWeldingProcessView::createSelectedProcess(Model::Process_Type p)
{
   switch (p)
   {
      case Model::MODEL_PROCESSO_MMA:

         PtrQuadro = new CWeldingContainerMma();
         add(*PtrQuadro);
         PtrQuadro->initialize();
         PtrQuadro->invalidate();
         PtrQuadro->setMenuSelectedView_Callback(setMenuSelectedContainer_Callback);
      break;
      case Model::MODEL_PROCESSO_TIG:
         //       cWeldingSetProcess.set_TA_Process_1(touchgfx::TypedText(VIPERDEF_PROCESSO_TIG));
         PtrQuadro = new CWeldingContainerTig();
         add(*PtrQuadro);
         PtrQuadro->initialize();
         PtrQuadro->invalidate();
         PtrQuadro->setMenuSelectedView_Callback(setMenuSelectedContainer_Callback);
      break;
      case Model::MODEL_PROCESSO_MIG:
         //       cWeldingSetProcess.set_TA_Process_1(touchgfx::TypedText(VIPERDEF_PROCESSO_MIG));
         PtrQuadro = new CWeldingContainerMig();
         add(*PtrQuadro);
         PtrQuadro->initialize();
         PtrQuadro->invalidate();
         PtrQuadro->setMenuSelectedView_Callback(setMenuSelectedContainer_Callback);
      break;

      case Model::MODEL_PROCESSO_MAX:
      default:
         if ( process >= Model::MODEL_PROCESSO_MAX )
            process = Model::MODEL_PROCESSO_MIG;
         else if ( process < Model::MODEL_PROCESSO_MMA )
            process = Model::MODEL_PROCESSO_MMA;
      break;
   }
}


void SWeldingProcessView::menucSelectedeContainer_Callback(U8 Menu,U8 process)
{


}

void SWeldingProcessView::encRX_incDec(S8 toward)
{
   if(!focusMenu)
   {
      if ( PtrQuadro )
      {
         remove(*PtrQuadro);
         _DELETE_(PtrQuadro);
      }
      process += toward;
      if ( process >= VIPERDEF_PROCESSO_MAX )
         process = VIPERDEF_PROCESSO_MIG;
      else if ( process < VIPERDEF_PROCESSO_MMA )
         process = VIPERDEF_PROCESSO_MMA;

      createSelectedProcess((Model::Process_Type)process);
   }
   else
   {
      if ( PtrQuadro )
      {
         PtrQuadro->seletcMenu(focusMenu,toward);
      }
   }
}



void SWeldingProcessView::encSX_incDec(S8 toward)
{
   focusMenu = toward;
   focusMenu = 0;
	if(PtrQuadro)
	{
		PtrQuadro->seletcMenu(focusMenu,0);
	}
}


void SWeldingProcessView::handleTickEvent()
{

//	switch(process)
//	{
//		default:
//		break;
//		case VIPERDEF_PROCESSO_MMA:
//			((CWeldingProcessMma*)PtrQuadro)->ManagerTickEvent();
//		break;
//		case VIPERDEF_PROCESSO_TIG:
//			((CWeldingProcessTig*)PtrQuadro)->ManagerTickEvent();
//		break;
//		case VIPERDEF_PROCESSO_MIG:
////			((CWeldingProcessMig*)PtrQuadro)->ManagerTickEvent();
//		break;
//	}
}

//void SWeldingProcessView::processChanged(uint8_t prc)
//{
//   process = prc;
//
//   if ( PtrQuadro )
//		{
//			remove(*PtrQuadro);
//			delete PtrQuadro;
//		}
//
//   switch (process)
//		{
//			default:
//			break;
//			case VIPERDEF_PROCESSO_MMA:
//				PtrQuadro = new CWeldingProcessMma();
//
//         if ( PtrQuadro )
//         {
//            ((CWeldingProcessMma*) PtrQuadro)->setXY(0, cInfoBar.getHeight());
//            add(*PtrQuadro);
//            ((CWeldingProcessMma*) PtrQuadro)->invalidate();
//         }
//      break;
//      case VIPERDEF_PROCESSO_TIG:
//         PtrQuadro = new CWeldingProcessTig();
//         if ( PtrQuadro )
//         {
//            ((CWeldingProcessTig*) PtrQuadro)->setXY(0, cInfoBar.getHeight());
//            add(*PtrQuadro);
//            ((CWeldingProcessTig*) PtrQuadro)->invalidate();
//         }
//      break;
//      case VIPERDEF_PROCESSO_MIG:
////         PtrQuadro = new CWeldingProcessMig();
//         if ( PtrQuadro )
//         {
////            ((CWeldingProcessMig*) PtrQuadro)
//            ((CWeldingProcessMig_*) PtrQuadro)->setXY(0, cInfoBar.getHeight());
//            add(*PtrQuadro);
//            ((CWeldingProcessMig_*) PtrQuadro)->invalidate();
//				}
//			break;
//		}
//}


void SWeldingProcessView::gasChanged(uint8_t type)
{
   /******** SCA dynami cast not permitted **********/
//   if(typeid(*PtrQuadro) == typeid(CWeldingProcessMig_))
//   {
//      ((CWeldingProcessMig_*)PtrQuadro)->setGas(type);
//   }
//   if(CWeldingProcessMig_* Ptr = dynamic_cast<CWeldingProcessMig_*>(PtrQuadro))
//   {
//      Ptr->setGas(type);
//   }
//   switch (process)
//   {
//      default:
//      break;
//      case VIPERDEF_PROCESSO_MMA:
//
//      break;
//      case VIPERDEF_PROCESSO_TIG:
//
//      break;
//      case VIPERDEF_PROCESSO_MIG:
////         ((CWeldingProcessMig*) PtrQuadro)->setGas(type);
//      break;
//   }
}


#if 0
void SWeldingProcessView::setMigConfiguration()
{

	pCWeldingSetParamContainer[0]->setVisible_TA_MainTitle(true);
   pCWeldingSetParamContainer[0]->setVisible_TA_Desc_1(false);
   pCWeldingSetParamContainer[0]->set_Color_BASE_1(color.setBackground_Focus());

   pCWeldingSetParamContainer[0]->setVisible_TA_Title_2(false);
   pCWeldingSetParamContainer[0]->setVisible_TA_Desc_2(false);
   pCWeldingSetParamContainer[0]->setVisible_IMG_iD_2(false);
   pCWeldingSetParamContainer[0]->setVisible_IMG_iTypo_2(false);

   pCWeldingSetParamContainer[0]->setVisible_TA_Title_3(false);
   pCWeldingSetParamContainer[0]->setVisible_TA_Desc_3(false);
   pCWeldingSetParamContainer[0]->setVisible_IMG_iD_3(true);
   pCWeldingSetParamContainer[0]->setVisible_IMG_iTypo_2(false);
   pCWeldingSetParamContainer[0]->set_IMG_iD_3(BITMAP_ICOTIG33X35_ID);

   pCWeldingSetParamContainer[0]->setActveBase(0, true);
   pCWeldingSetParamContainer[0]->setActveBase(1, false);
   pCWeldingSetParamContainer[0]->setActveBase(2, false);
   pCWeldingSetParamContainer[0]->set_TA_MainTitle(T_PROC_MIG);

}

void SWeldingProcessView::setTigConfiguration()
{

}
void SWeldingProcessView::setWireCB()
{

}
#endif
