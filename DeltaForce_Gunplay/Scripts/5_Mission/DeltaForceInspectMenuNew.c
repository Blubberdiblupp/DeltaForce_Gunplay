modded class InspectMenuNew
{
	protected Widget m_DeltaForceInspectStatsDisplay;
	
	override void SetItem(EntityAI item)
	{
		super.SetItem(item);
		UpdateDeltaForceInspectStats(item);
	}
	
	protected void UpdateDeltaForceInspectStats(EntityAI item)
	{
		EnsureDeltaForceInspectStatsDisplay();
		if (!m_DeltaForceInspectStatsDisplay)
		{
			return;
		}
		
		DeltaForceGunplayAdminConfig adminConfig = DeltaForceGetAdminConfig();
		if (adminConfig && !adminConfig.EnableInspectStats)
		{
			m_DeltaForceInspectStatsDisplay.Show(false);
			return;
		}
		
		DeltaForceDisplayData displayData = DeltaForceDisplayStats.GetDisplay(item);
		if (!displayData)
		{
			m_DeltaForceInspectStatsDisplay.Show(false);
			return;
		}
		
		UpdateDeltaForceInspectStatsDisplay(displayData);
		
		m_DeltaForceInspectStatsDisplay.Show(true);
		HideCompetingDeltaForceInspectStatsDisplay();
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.HideCompetingDeltaForceInspectStatsDisplay, 1, false);
	}
	
	protected void UpdateDeltaForceInspectStatsDisplay(DeltaForceDisplayData displayData)
	{
		SetDeltaForceInspectText("tier_value", displayData.TierLabel, 0xFFFFFFFF);
		SetDeltaForceInspectText("recoil_label", displayData.Line1Label, 0xFFAF9442);
		SetDeltaForceInspectText("recoil_value", displayData.Line1Value, displayData.Line1Color);
		SetDeltaForceInspectText("sway_label", displayData.Line2Label, 0xFFAF9442);
		SetDeltaForceInspectText("sway_value", displayData.Line2Value, displayData.Line2Color);
		SetDeltaForceInspectText("ads_label", displayData.Line3Label, 0xFFAF9442);
		SetDeltaForceInspectText("ads_value", displayData.Line3Value, displayData.Line3Color);
		SetDeltaForceInspectText("precision_label", displayData.Line4Label, 0xFFAF9442);
		SetDeltaForceInspectText("precision_value", displayData.Line4Value, displayData.Line4Color);
		SetDeltaForceInspectText("hipfire_label", displayData.Line5Label, 0xFFAF9442);
		SetDeltaForceInspectText("hipfire_value", displayData.Line5Value, displayData.Line5Color);
		SetDeltaForceInspectLineVisible("hipfire", displayData.ShowLine5);
	}
	
	protected void EnsureDeltaForceInspectStatsDisplay()
	{
		if (m_DeltaForceInspectStatsDisplay)
		{
			return;
		}
		
		if (!layoutRoot)
		{
			return;
		}
		
		Widget parent = layoutRoot.FindAnyWidget("ItemFrameWidgetPanel");
		if (!parent)
		{
			parent = layoutRoot;
		}
		
		m_DeltaForceInspectStatsDisplay = GetGame().GetWorkspace().CreateWidgets("DeltaForce_Gunplay/GUI/layouts/DeltaForceItemStatsInspect.layout", parent);
		if (m_DeltaForceInspectStatsDisplay)
		{
			m_DeltaForceInspectStatsDisplay.Show(false);
		}
	}
	
	protected void SetDeltaForceInspectText(string widgetName, string text, int color)
	{
		TextWidget widget = TextWidget.Cast(m_DeltaForceInspectStatsDisplay.FindAnyWidget(widgetName));
		if (!widget)
		{
			return;
		}
		
		widget.SetText(text);
		widget.SetColor(color);
	}
	
	protected void SetDeltaForceInspectLineVisible(string widgetName, bool visible)
	{
		Widget widget = m_DeltaForceInspectStatsDisplay.FindAnyWidget(widgetName);
		if (widget)
		{
			widget.Show(visible);
		}
	}
	
	protected void HideCompetingDeltaForceInspectStatsDisplay()
	{
		if (!layoutRoot)
		{
			return;
		}
		
		Widget snafuStatsDisplay = layoutRoot.FindAnyWidget("SNAFUStatsDisplay");
		if (snafuStatsDisplay)
		{
			snafuStatsDisplay.Show(false);
		}
	}
}
