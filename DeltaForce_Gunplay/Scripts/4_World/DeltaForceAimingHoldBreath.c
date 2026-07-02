modded class DayZPlayerImplementAiming
{
	protected bool m_DeltaForceHoldBreathFreezeActive;
	protected float m_DeltaForceHoldBreathFreezeX;
	protected float m_DeltaForceHoldBreathFreezeY;

	override void OnRaiseBegin(DayZPlayerImplement player)
	{
		super.OnRaiseBegin(player);
		
		if (!player)
		{
			return;
		}
		
		Weapon_Base weapon = Weapon_Base.Cast(player.GetHumanInventory().GetEntityInHands());
		DeltaForceRegisterAimSpeedTransitionTimes(weapon);
	}

	protected void DeltaForceRegisterAimSpeedTransitionTimes(Weapon_Base weapon)
	{
		if (!weapon)
		{
			return;
		}
		
		if (!weapon.DeltaForceShouldApplyGunplay())
		{
			return;
		}
		
		float transitionTime = weapon.GetDeltaForceOpticEnterDelay() / 1000.0;
		DayZPlayerCameras.RegisterTransitionTime(DayZPlayerCameras.DAYZCAMERA_1ST, DayZPlayerCameras.DAYZCAMERA_IRONSIGHTS, transitionTime, false);
		DayZPlayerCameras.RegisterTransitionTime(DayZPlayerCameras.DAYZCAMERA_1ST, DayZPlayerCameras.DAYZCAMERA_OPTICS, transitionTime, false);
		DayZPlayerCameras.RegisterTransitionTime(DayZPlayerCameras.DAYZCAMERA_IRONSIGHTS, DayZPlayerCameras.DAYZCAMERA_OPTICS, transitionTime, true);
	}

	override bool ProcessAimFilters(float pDt, SDayZPlayerAimingModel pModel, int stance_index)
	{
		bool shouldFreezeHoldBreath = DeltaForceShouldFreezeHoldBreathSway();
		if (!shouldFreezeHoldBreath)
		{
			m_DeltaForceHoldBreathFreezeActive = false;
			return super.ProcessAimFilters(pDt, pModel, stance_index);
		}

		if (!m_DeltaForceHoldBreathFreezeActive)
		{
			bool firstResult = AJProcessAimFiltersWithVanillaHoldBreathSpeed(pDt, pModel, stance_index);
			m_DeltaForceHoldBreathFreezeX = pModel.m_fAimXHandsOffset;
			m_DeltaForceHoldBreathFreezeY = pModel.m_fAimYHandsOffset;
			m_DeltaForceHoldBreathFreezeActive = true;
			return firstResult;
		}

		bool oldAimNoiseAllowed = IsAimNoiseAllowed();
		vector oldSwayModifier = m_SwayModifier;
		float oldHorizontalNoiseXAxisOffset = m_HorizontalNoiseXAxisOffset;
		float oldBreathingXAxisOffset = m_BreathingXAxisOffset;
		float oldBreathingYAxisOffset = m_BreathingYAxisOffset;

		SetAimNoiseAllowed(false);
		m_SwayModifier[2] = 1.0;
		m_HorizontalNoiseXAxisOffset = 0.0;
		m_BreathingXAxisOffset = 0.0;
		m_BreathingYAxisOffset = 0.0;

		bool result = super.ProcessAimFilters(pDt, pModel, stance_index);

		pModel.m_fAimXHandsOffset = pModel.m_fAimXHandsOffset + m_DeltaForceHoldBreathFreezeX;
		pModel.m_fAimYHandsOffset = pModel.m_fAimYHandsOffset + m_DeltaForceHoldBreathFreezeY;

		float absAimY = Math.AbsFloat(pModel.m_fCurrentAimY);
		pModel.m_fAimYHandsOffset = Math.Clamp(pModel.m_fAimYHandsOffset, absAimY - 89.9, 89.9 - absAimY);

		SetAimNoiseAllowed(oldAimNoiseAllowed);
		m_SwayModifier = oldSwayModifier;
		m_HorizontalNoiseXAxisOffset = oldHorizontalNoiseXAxisOffset;
		m_BreathingXAxisOffset = oldBreathingXAxisOffset;
		m_BreathingYAxisOffset = oldBreathingYAxisOffset;

		return result;
	}

	protected bool AJProcessAimFiltersWithVanillaHoldBreathSpeed(float pDt, SDayZPlayerAimingModel pModel, int stance_index)
	{
		vector oldSwayModifier = m_SwayModifier;
		m_SwayModifier[2] = 1.0;

		bool result = super.ProcessAimFilters(pDt, pModel, stance_index);

		m_SwayModifier = oldSwayModifier;
		return result;
	}

	protected bool DeltaForceShouldFreezeHoldBreathSway()
	{
		if (!DeltaForceShouldUseVanillaHoldBreathSpeed())
		{
			return false;
		}

		return true;
	}
	
	protected bool DeltaForceShouldUseVanillaHoldBreathSpeed()
	{
		if (!m_PlayerPb)
		{
			return false;
		}
		
		if (!m_PlayerPb.IsHoldingBreath())
		{
			return false;
		}
		
		if (!(m_PlayerPb.IsInIronsights() || m_PlayerPb.IsInOptics()))
		{
			return false;
		}
		
		Weapon_Base weapon = Weapon_Base.Cast(m_PlayerPb.GetItemInHands());
		if (!weapon)
		{
			return false;
		}
		
		return weapon.DeltaForceShouldApplyGunplay();
	}
}
