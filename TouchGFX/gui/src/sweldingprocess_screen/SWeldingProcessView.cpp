#include <gui/sweldingprocess_screen/SWeldingProcessView.hpp>
#include <gui/custom/CWeldingContainerMig.hpp>
#include <gui/custom/CWeldingContainerMma.hpp>
#include <gui/custom/CWeldingContainerTig.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>



SWeldingProcessView::SWeldingProcessView():PtrQuadro(NULL),setInitContainer_Callback(this,&SWeldingProcessView::fromContainerInitContainer_Callback),\
setMenuSelectedContainer_Callback(this,&SWeldingProcessView::fromContainerMenuSelected_Callback),process(0),focusMenu(0)
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

   if ( process >= VIPERDEF_PROCESSO_MAX )
      process = VIPERDEF_PROCESSO_MIG;
   else if ( process < VIPERDEF_PROCESSO_MMA )
      process = VIPERDEF_PROCESSO_MMA;

   createSelectedProcess(viperdef_Processo_e(process));

   focusMenu = VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO;
}



void SWeldingProcessView::tearDownScreen()
{
    SWeldingProcessViewBase::tearDownScreen();
}

void SWeldingProcessView::GoBack()
{
   application().VisualizzaSMenuPrincipale();
}




void SWeldingProcessView::createSelectedProcess(viperdef_Processo_e p)
{
   if ( PtrQuadro )
   {
      remove(*PtrQuadro);
      _DELETE_(PtrQuadro);
   }

   switch (p)
   {
      case VIPERDEF_PROCESSO_MMA:

         PtrQuadro = new CWeldingContainerMma();
         PtrQuadro->setMenuSelectedView_Callback(setMenuSelectedContainer_Callback);
         add(*PtrQuadro);
         PtrQuadro->initialize();
         PtrQuadro->invalidate();
         presenter->setProcessToModel(p);
      break;
      case VIPERDEF_PROCESSO_TIG:
         //       cWeldingSetProcess.set_TA_Process_1(touchgfx::TypedText(VIPERDEF_PROCESSO_TIG));
         PtrQuadro = new CWeldingContainerTig();
         PtrQuadro->setMenuSelectedView_Callback(setMenuSelectedContainer_Callback);
         add(*PtrQuadro);
         PtrQuadro->initialize();
         PtrQuadro->invalidate();
         presenter->setProcessToModel(p);
      break;
      case VIPERDEF_PROCESSO_MIG:
         //       cWeldingSetProcess.set_TA_Process_1(touchgfx::TypedText(VIPERDEF_PROCESSO_MIG));
         PtrQuadro = new CWeldingContainerMig();
         PtrQuadro->setMenuSelectedView_Callback(setMenuSelectedContainer_Callback);
         PtrQuadro->setInitContainer_Callback(setInitContainer_Callback);
         add(*PtrQuadro);
         PtrQuadro->initialize();
         PtrQuadro->invalidate();
         presenter->setProcessToModel(p);
      break;

      case VIPERDEF_PROCESSO_MAX:
      default:
//         if ( process >= VIPERDEF_PROCESSO_MAX )
//            process = VIPERDEF_PROCESSO_MIG;
//         else if ( process < VIPERDEF_PROCESSO_MMA )
//            process = VIPERDEF_PROCESSO_MMA;
      break;
   }
}


void SWeldingProcessView::encRXconfirm(void)
{
//   if(focusMenu == VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO)
//   {
//      presenter->setProcessToModel(viperdef_TipoMig_e(process));
//      focusMenu = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG;
//   }
//   else
//      focusMenu +=1;
}

/***********************Calback*************************/
void SWeldingProcessView::fromContainerInitContainer_Callback(void)
{
   if ( PtrQuadro->getContainerType() == VIPERDEF_PROCESSO_MIG )
   {
      presenter->getWireDiameter();
      presenter->getWireType();
      presenter->getGasType();
      presenter->getMigType();
   }
}
void SWeldingProcessView::fromContainerMenuSelected_Callback(S8 Menu,S8 Process)
{
   presenter->setFocusIdVariableToModel(Menu, Process);
}

void SWeldingProcessView::encRX_incDec(S8 toward)
{
/*
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
      presenter->setFocusIdVariableToModel(0, process);
      createSelectedProcess(viperdef_TipoMig_e(process));
   }
   */
//   else
//   {
//      if ( PtrQuadro )
//      {
//         PtrQuadro->seletcMenu(focusMenu,toward);
//      }
//   }
}

void SWeldingProcessView::setMenuContainer(viperui_AttualeSelezioneProcesso_e m)
{
   if(PtrQuadro)
   {
      PtrQuadro->seletcMenu(m,0);
   }
}

void SWeldingProcessView::encSX_incDec(S8 toward)
{
//   if(focusMenu == VIPERUI_ATUALESELEZIONEPROCESSO_TIPOPROCESSO)
//      focusMenu = VIPERUI_ATUALESELEZIONEPROCESSO_MIG_TIPOMIG;
//   else
//   focusMenu += toward;

//	if(PtrQuadro)
//	{
//		PtrQuadro->seletcMenu(focusMenu,0);
//	}
}



void SWeldingProcessView::updateWireType(viperdef_TipoFilo_e type)
{
   if(PtrQuadro->getContainerType() == VIPERDEF_PROCESSO_MIG)
   {
      PtrQuadro->seletcMig_WireType(type);
   }
}

void SWeldingProcessView::updateWireDiameter(viperdef_DiametroFilo_e type)
{
   if(PtrQuadro->getContainerType() == VIPERDEF_PROCESSO_MIG)
   {
      PtrQuadro->seletcMig_WireDIameter(type);
   }
}

void SWeldingProcessView::updateGasType(viperdef_TipoGas_e type)
{
   if(PtrQuadro->getContainerType() == VIPERDEF_PROCESSO_MIG)
   {
      PtrQuadro->seletcMig_GasType(type);
   }
}

void SWeldingProcessView::updateMigType(viperdef_TipoMig_e type)
{
   if(PtrQuadro->getContainerType() == VIPERDEF_PROCESSO_MIG)
   {
      PtrQuadro->seletcMig_MigType(type);
   }
}
