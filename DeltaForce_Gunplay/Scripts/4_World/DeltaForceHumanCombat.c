modded class DayZPlayerImplement
{
	override void SwitchOptics(ItemOptics optic, bool state)
	{
		PlayerBase player = PlayerBase.Cast(this);
		Weapon_Base weapon;
		if (player)
		{
			weapon = Weapon_Base.Cast(player.GetItemInHands());
		}
		
		if (!optic || !weapon || !weapon.DeltaForceShouldApplyGunplay())
		{
			super.SwitchOptics(optic, state);
			return;
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).RemoveByName(optic, "OnOpticEnter");
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).RemoveByName(optic, "EnterOptics");
		
		if (state)
		{
			int delay = weapon.GetDeltaForceOpticEnterActionDelay();
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLaterByName(optic, "EnterOptics", delay, false);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLaterByName(optic, "OnOpticEnter", delay, false);
		}
		else
		{
			optic.ExitOptics();
			optic.OnOpticExit();
			
			if (optic.HasEnergyManager())
			{
				optic.GetCompEM().SwitchOff();
			}
		}
		
		if (m_CameraOptics != state)
		{
			SetOptics(state);
		}
	}
	
	override bool IsShootingFromCamera()
	{
		DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
		if (!config)
		{
			return super.IsShootingFromCamera();
		}

		if (!config.EnableHipFireNerf)
		{
			return super.IsShootingFromCamera();
		}
		
		DeltaForceGunplayAdminConfig adminConfig = DeltaForceGetAdminConfig();
		if (adminConfig && adminConfig.CrosshairMode == 0)
		{
			return super.IsShootingFromCamera();
		}

		PlayerBase player = PlayerBase.Cast(this);
		if (player && DeltaForceVehicleCombatHandler.IsPlayerInVehicle(player))
		{
			return super.IsShootingFromCamera();
		}
		
		if (player)
		{
			Weapon_Base weapon = Weapon_Base.Cast(player.GetItemInHands());
			if (DeltaForceIsInAimingView(player, weapon))
			{
				return super.IsShootingFromCamera();
			}
			
			if (weapon && !weapon.DeltaForceShouldApplyGunplay())
			{
				return super.IsShootingFromCamera();
			}
			
			if (adminConfig && adminConfig.CrosshairMode == 2 && !DeltaForceHasHipfireLaser(weapon))
			{
				return super.IsShootingFromCamera();
			}
		}

		return false;
	}
	
	protected bool DeltaForceIsInAimingView(PlayerBase player, Weapon_Base weapon)
	{
		if (m_bADS || m_CameraIronsight || m_CameraOptics)
		{
			return true;
		}
		
		if (player && (player.IsInIronsights() || player.IsInOptics()))
		{
			return true;
		}
		
		if (weapon && weapon.IsInOptics())
		{
			return true;
		}
		
		return false;
	}
	
	protected bool DeltaForceHasHipfireLaser(Weapon_Base weapon)
	{
		if (!weapon)
		{
			return false;
		}
		
		if (weapon.FindAttachmentBySlotName("weaponLaserPointer")) return true;
		if (weapon.FindAttachmentBySlotName("weaponLaser")) return true;
		if (weapon.FindAttachmentBySlotName("PistolLaserPointer")) return true;
		if (weapon.FindAttachmentBySlotName("AJPEQLASER")) return true;
		if (weapon.FindAttachmentBySlotName("Expansion_ANPEQ15_Base")) return true;
		
		if (weapon.GetInventory())
		{
			for (int i = 0; i < weapon.GetInventory().AttachmentCount(); i++)
			{
				EntityAI attachment = weapon.GetInventory().GetAttachmentFromIndex(i);
				if (attachment && DeltaForceIsLaserType(attachment.GetType()))
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	protected bool DeltaForceIsLaserType(string itemType)
	{
		if (itemType.Contains("Laser")) return true;
		if (itemType.Contains("PEQ")) return true;
		if (itemType.Contains("ANPEQ")) return true;
		if (itemType.Contains("NGAL")) return true;
		if (itemType.Contains("DBAL")) return true;
		if (itemType.Contains("MAWL")) return true;
		
		return false;
	}
}
