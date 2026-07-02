modded class Weapon_Base
{
	protected bool m_WeaponGeometryDamageEnabled;
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
		m_WeaponGeometryDamageEnabled = config.EnableWeaponGeometryDamage;
	}
	
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
		
		if (!config.EnableWeaponGeometryDamage)
		{
			if (damageType == DamageType.FIRE_ARM)
			{
				if (config.DebugMode)
				{
					Print("[DeltaForce Gunplay] Weapon geometry damage BLOCKED for: " + GetType());
				}
				
				return false;
			}
		}
		
		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	
	override void SetHealth(float health)
	{
		DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
		
		if (!config.EnableWeaponGeometryDamage)
		{
			if (health < GetHealth())
			{
				PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
				if (player && GetHierarchyRoot() == player.GetHumanInventory().GetEntityInHands())
				{
					if (config.DebugMode)
					{
						Print("[DeltaForce Gunplay] Weapon health reduction prevented");
					}
					return;
				}
			}
		}
		
		super.SetHealth(health);
	}
	
	override void EEKilled(Object killer)
	{
		DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
		
		if (!config.EnableWeaponGeometryDamage)
		{
			if (config.DebugMode)
			{
				Print("[DeltaForce Gunplay] Weapon ruined event prevented");
			}
			
			SetHealth(GetMaxHealth());
			return;
		}
		
		super.EEKilled(killer);
	}
}
