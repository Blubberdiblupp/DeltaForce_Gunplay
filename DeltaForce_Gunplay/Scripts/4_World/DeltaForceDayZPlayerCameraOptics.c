modded class DayZPlayerCameraOptics
{
	override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
	{
		super.OnActivate(pPrevCamera, pPrevCameraResult);
		
		PlayerBase player = PlayerBase.Cast(m_pPlayer);
		if (!player || !m_opticsUsed)
		{
			return;
		}
		
		Weapon_Base weapon = Weapon_Base.Cast(player.GetItemInHands());
		if (!weapon || !weapon.DeltaForceShouldApplyGunplay())
		{
			return;
		}
		
		int delay = weapon.GetDeltaForceOpticEnterDelay();
		int ppDelay = delay - 100;
		if (ppDelay < 0)
		{
			ppDelay = 0;
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(player.HideClothing);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(SetCameraPP);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetCameraPP, ppDelay, false, true, this);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(player.HideClothing, delay, false, m_opticsUsed, true);
	}
}
