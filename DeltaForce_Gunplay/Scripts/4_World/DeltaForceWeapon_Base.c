modded class Weapon_Base
{
	protected ref DeltaForceWeaponStatsManager m_DeltaForceStatsManager;
	protected bool m_DeltaForceStatsDirty = true;
	
	protected bool m_DeltaForceWasInIronsights;
	protected int m_DeltaForceLastZeroing;
	protected float m_DeltaForceLastZoomLevel;
	
	void Weapon_Base()
	{
		m_DeltaForceStatsManager = new DeltaForceWeaponStatsManager();
		m_DeltaForceWasInIronsights = false;
		m_DeltaForceLastZeroing = 0;
		m_DeltaForceLastZoomLevel = 0;
	}
	
	void ~Weapon_Base()
	{
		if (m_DeltaForceStatsManager)
		{
			delete m_DeltaForceStatsManager;
		}
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		if (item)
		{
			m_DeltaForceStatsDirty = true;
		}
		
		super.EEItemAttached(item, slot_name);
		
		if (!item) return;
		
		string itemType = item.GetType();
		
		DeltaForceNetworkOptimizer optimizer = DeltaForceNetworkOptimizer.GetInstance();
		
		if (!optimizer.ShouldTriggerStatRecalc(itemType, slot_name))
		{
			if (GetGame().IsServer())
			{
				DeltaForceGunplayConfig debugConfigAttached = DeltaForceGunplayConfig.GetInstance();
				if (debugConfigAttached.DebugMode)
				{
					Print("[DeltaForce Gunplay] Cosmetic attachment ignored: " + itemType + " (slot: " + slot_name + ")");
				}
			}
			return;
		}
		
		if (GetGame().IsServer())
		{
			DeltaForceGunplayConfig attachConfig = DeltaForceGunplayConfig.GetInstance();
			if (attachConfig.DebugMode)
			{
				Print("[DeltaForce Gunplay] Stat-affecting item attached: " + itemType + " to slot: " + slot_name);
			}
			
			DeltaForceRecalculateStats();
		}
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		if (item)
		{
			m_DeltaForceStatsDirty = true;
		}
		
		super.EEItemDetached(item, slot_name);
		
		if (!item) return;
		
		string itemType = item.GetType();
		
		DeltaForceNetworkOptimizer optimizer = DeltaForceNetworkOptimizer.GetInstance();
		
		if (!optimizer.ShouldTriggerStatRecalc(itemType, slot_name))
		{
			if (GetGame().IsServer())
			{
				DeltaForceGunplayConfig debugConfigDetached = DeltaForceGunplayConfig.GetInstance();
				if (debugConfigDetached.DebugMode)
				{
					Print("[DeltaForce Gunplay] Cosmetic detachment ignored: " + itemType + " (slot: " + slot_name + ")");
				}
			}
			return;
		}
		
		if (GetGame().IsServer())
		{
			DeltaForceGunplayConfig detachConfig = DeltaForceGunplayConfig.GetInstance();
			if (detachConfig.DebugMode)
			{
				Print("[DeltaForce Gunplay] Stat-affecting item detached: " + itemType + " from slot: " + slot_name);
			}
			
			DeltaForceRecalculateStats();
		}
	}
	
	void DeltaForceRecalculateStats()
	{
		if (!m_DeltaForceStatsManager)
		{
			m_DeltaForceStatsManager = new DeltaForceWeaponStatsManager();
		}
		
		m_DeltaForceStatsManager.CalculateStats(this);
		m_DeltaForceStatsDirty = false;
		
		DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
		
		if (config.DebugMode && m_DeltaForceStatsManager)
		{
			Print(m_DeltaForceStatsManager.GetStatsString());
		}
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		if (DeltaForceShouldApplyGunplay() && m_DeltaForceStatsDirty)
		{
			DeltaForceRecalculateStats();
		}
		
		return super.SpawnRecoilObject();
	}
	
	float GetDeltaForceWeaponRecoilModifier()
	{
		if (!DeltaForceShouldApplyGunplay())
		{
			return 1.0;
		}
		
		if (m_DeltaForceStatsDirty)
		{
			DeltaForceRecalculateStats();
		}
		
		if (m_DeltaForceStatsManager)
		{
			return m_DeltaForceStatsManager.GetCurrentRecoil();
		}
		
		return 1.0;
	}
	
	float GetDeltaForceAimingSwayModifier()
	{
		if (!DeltaForceShouldApplyGunplay())
		{
			return 1.0;
		}
		
		if (m_DeltaForceStatsDirty)
		{
			DeltaForceRecalculateStats();
		}
		
		if (m_DeltaForceStatsManager)
		{
			float rawSway = m_DeltaForceStatsManager.GetCurrentSway() / m_DeltaForceStatsManager.GetCurrentPrecision();
			return DeltaForceGetADSSafeSwayModifier(rawSway);
		}
		
		return 1.0;
	}
	
	float DeltaForceGetADSSafeSwayModifier(float rawSway)
	{
		return DeltaForceClampFloat(rawSway, 0.15, 1.0);
	}
	
	float GetDeltaForceAimingSwaySpeedModifier()
	{
		if (!DeltaForceShouldApplyGunplay())
		{
			return 1.0;
		}
		
		if (m_DeltaForceStatsDirty)
		{
			DeltaForceRecalculateStats();
		}
		
		if (m_DeltaForceStatsManager)
		{
			float rawSway = m_DeltaForceStatsManager.GetCurrentSway() / m_DeltaForceStatsManager.GetCurrentPrecision();
			return DeltaForceGetADSSafeSwaySpeedModifier(rawSway);
		}
		
		return 1.0;
	}
	
	float DeltaForceGetADSSafeSwaySpeedModifier(float rawSway)
	{
		return DeltaForceClampFloat(rawSway, 0.35, 1.0);
	}
	
	float DeltaForceClampFloat(float value, float minValue, float maxValue)
	{
		if (value < minValue)
		{
			return minValue;
		}
		
		if (value > maxValue)
		{
			return maxValue;
		}
		
		return value;
	}
	
	float GetDeltaForceOpticsDisableLookOverride()
	{
		if (!DeltaForceShouldApplyGunplay())
		{
			return 1.0;
		}
		
		if (m_DeltaForceStatsDirty)
		{
			DeltaForceRecalculateStats();
		}
		
		if (m_DeltaForceStatsManager)
		{
			return m_DeltaForceStatsManager.GetCurrentAimSpeed();
		}
		
		return 1.0;
	}
	
	int GetDeltaForceOpticEnterDelay()
	{
		float aimSpeed = GetDeltaForceOpticsDisableLookOverride();
		if (aimSpeed <= 0.05)
		{
			aimSpeed = 1.0;
		}
		
		float delay = 700.0 / aimSpeed;
		if (delay < 150.0)
		{
			delay = 150.0;
		}
		
		if (delay > 1400.0)
		{
			delay = 1400.0;
		}
		
		return Math.Round(delay);
	}
	
	int GetDeltaForceOpticEnterActionDelay()
	{
		int delay = GetDeltaForceOpticEnterDelay() - 700;
		if (delay < 0)
		{
			return 0;
		}
		
		return delay;
	}
	
	float GetDeltaForceHipFireSpreadModifier()
	{
		if (!DeltaForceShouldApplyGunplay())
		{
			return 1.0;
		}
		
		if (m_DeltaForceStatsDirty)
		{
			DeltaForceRecalculateStats();
		}
		
		if (m_DeltaForceStatsManager)
		{
			return m_DeltaForceStatsManager.GetDeltaForceHipFireSpreadModifier();
		}
		
		return 1.0;
	}
	
	bool DeltaForceShouldApplyGunplay()
	{
		if (DeltaForceGunplayOwnership.OwnsCommonGunplay())
		{
			return true;
		}
		
		return DeltaForceHasWeaponPackContent();
	}
	
	bool DeltaForceHasWeaponPackContent()
	{
		if (DeltaForceGunplayOwnership.IsDeltaForceClass(GetType()))
		{
			return true;
		}
		
		
		if (!GetInventory())
		{
			return false;
		}
		
		for (int i = 0; i < GetInventory().AttachmentCount(); i++)
		{
			EntityAI attachment = GetInventory().GetAttachmentFromIndex(i);
			if (attachment && DeltaForceGunplayOwnership.IsDeltaForceClass(attachment.GetType()))
			{
				return true;
			}
		}
		
		return false;
	}
	
	bool DeltaForceHasGripAttached()
	{
		if (FindAttachmentBySlotName("FGRIP")) return true;
		if (FindAttachmentBySlotName("weaponGripCQR")) return true;
		if (FindAttachmentBySlotName("weaponGripKAAR")) return true;
		
		return false;
	}
	
	bool DeltaForceHasScopeAttached()
	{
		if (FindAttachmentBySlotName("weaponOptics")) return true;
		if (FindAttachmentBySlotName("weaponOpticsHunting")) return true;
		if (FindAttachmentBySlotName("weaponOpticsSniper")) return true;
		
		return false;
	}
	
	bool DeltaForceIsHeavyScopeAttached()
	{
		return false;
	}
	
	string GetDeltaForceGunplayStatsDisplay()
	{
		if (m_DeltaForceStatsDirty)
		{
			DeltaForceRecalculateStats();
		}
		
		if (m_DeltaForceStatsManager)
		{
			return m_DeltaForceStatsManager.GetStatsString();
		}
		
		return "";
	}
	
}
