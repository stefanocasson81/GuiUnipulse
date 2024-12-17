#include <gui/smain_screen/SMainView.hpp>
#include <gui/containers/CMainMig.hpp>
#include <gui/containers/CMainMma.hpp>
#include <gui/containers/CMainTig.hpp>
#include <images/BitmapDatabase.hpp>


extern "C"
{
	#include "ARMLib/Common/ComDef.h"
	#include "CORELib/Common/ViperDef.h"
	#include "_Board.h"
	#include "CORELib/UserInterface/ViperUI.h"
};



SMainView::SMainView()
{
	PtrQuadro = _NULL_;
	ShowProcesso = VIPERDEF_PROCESSO_MAX;
	ShowTipoMig = VIPERDEF_TIPOMIG_MAX;

	cStrumento.setVisible(false);
	cStrumento.setXY(0, 0);
	cStrumento.invalidate();

	cAllarmi.setVisible(false);
	cAllarmi.setXY(0, 0);
	cAllarmi.invalidate();

	handleTickEvent();	// Prima rinfrescata per generare bene il quadro

	//touchgfx::Application::getInstance()->registerTimerWidget(this);	// Ricordarsi di fare il unregisterTimerWidget sul distruttore
}

SMainView::~SMainView()
{
	if(PtrQuadro)
	{
		delete PtrQuadro;
		PtrQuadro = _NULL_;
		ShowProcesso = VIPERDEF_PROCESSO_MAX;
	}

	//touchgfx::Application::getInstance()->unregisterTimerWidget(this);
}

void SMainView::handleTickEvent()
{
	viperdef_Processo_e processo;
	viperdef_TipoMig_e tipoMig;
	bool rigenera = false;

	processo = (viperdef_Processo_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_BASE_PROCESSO]);
	tipoMig = (viperdef_TipoMig_e)data_GetValData(&viperui_StrutturaDati[VIPERUI_DATA_BASE_TIPOMIG]);

	// Un minimo di protezione su valori sballati
	if(processo<VIPERDEF_PROCESSO_MAX)
	{
		if((processo!=ShowProcesso) || (tipoMig!=ShowTipoMig))
		{
			rigenera = true;

			if(PtrQuadro)
			{
				remove(*PtrQuadro);
				_DELETE_(PtrQuadro);
			}

			switch(processo)
			{
				default:
				break;
				case VIPERDEF_PROCESSO_MMA:
					cInfoBar.SetTitolo((char*)"MMA");

					PtrQuadro = new CMainMma();
					if(PtrQuadro)
					{
						((CMainMma*)PtrQuadro)->setXY(0,cInfoBar.getHeight());
						add(*PtrQuadro);
						((CMainMma*)PtrQuadro)->invalidate();
					}
				break;
				case VIPERDEF_PROCESSO_TIG:
					cInfoBar.SetTitolo((char*)"TIG");

					PtrQuadro = new CMainTig();
					if(PtrQuadro)
					{
						((CMainTig*)PtrQuadro)->setXY(0,cInfoBar.getHeight());
						add(*PtrQuadro);
						((CMainTig*)PtrQuadro)->invalidate();
					}
				break;
				case VIPERDEF_PROCESSO_MIG:
					switch(viperdef_Pack8GenTx_Base.TipoMig)
					{
						default:
							cInfoBar.SetTitolo((char*)"XXX");
						break;

						case VIPERDEF_TIPOMIG_MAN:
							cInfoBar.SetTitolo((char*)"MIG MANUAL");
						break;
						case VIPERDEF_TIPOMIG_SYN:
							cInfoBar.SetTitolo((char*)"MIG SYNERGIC");
						break;
						case VIPERDEF_TIPOMIG_PULSE:
							cInfoBar.SetTitolo((char*)"MIG PULSED");
						break;
					}

					PtrQuadro = new CMainMig();
					if(PtrQuadro)
					{
						((CMainMig*)PtrQuadro)->setXY(0,cInfoBar.getHeight());
						add(*PtrQuadro);
						((CMainMig*)PtrQuadro)->invalidate();
					}
				break;
			}

			switch(processo)
			{
				default:
				break;
				case VIPERDEF_PROCESSO_MMA:
					cInfoBar.SetIco(BITMAP_ICOMMA43X35_ID);
				break;
				case VIPERDEF_PROCESSO_TIG:
					cInfoBar.SetIco(BITMAP_ICOTIG33X35_ID);
				break;
				case VIPERDEF_PROCESSO_MIG:
					cInfoBar.SetIco(BITMAP_ICOMIG38X35_ID);
				break;
			}

			ShowProcesso = processo;
			ShowTipoMig = tipoMig;
		}

		switch(processo)
		{
			default:
			break;
			case VIPERDEF_PROCESSO_MMA:
				((CMainMma*)PtrQuadro)->ManagerTickEvent();
			break;
			case VIPERDEF_PROCESSO_TIG:
				((CMainTig*)PtrQuadro)->ManagerTickEvent();
			break;
			case VIPERDEF_PROCESSO_MIG:
				((CMainMig*)PtrQuadro)->ManagerTickEvent();
			break;
		}
	}

	// L'effettiva visualizzazione dello strumento e allarmi è gestita all'interno dei ManagerTickEvent
	if(cStrumento.ManagerTickEvent() || rigenera)
	{	// Accrocchio per mettere in primo piano la finestra
		remove(cStrumento);
		add(cStrumento);
		invalidate();
	}
	if(cAllarmi.ManagerTickEvent() || rigenera)
	{	// Accrocchio per mettere in primo piano la finestra
		remove(cAllarmi);
		add(cAllarmi);
		invalidate();
	}

	cInfoBar.ManagerTickEvent();
}


void SMainView::setupScreen()
{
    SMainViewBase::setupScreen();
}

void SMainView::tearDownScreen()
{
    SMainViewBase::tearDownScreen();
}
