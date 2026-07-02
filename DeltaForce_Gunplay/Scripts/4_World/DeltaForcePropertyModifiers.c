modded class PropertyModifiers
{
	override void UpdateModifiers()
	{
		super.UpdateModifiers();
		
		Weapon_Base weapon = Weapon_Base.Cast(m_OwnerItem);
		if (!weapon)
		{
			return;
		}
		
		if (!weapon.DeltaForceShouldApplyGunplay())
		{
			return;
		}
		
		float recoilModifier = weapon.GetDeltaForceWeaponRecoilModifier();
		float swayModifier = weapon.GetDeltaForceAimingSwayModifier();
		float swaySpeedModifier = weapon.GetDeltaForceAimingSwaySpeedModifier();
		
		if (recoilModifier > 0.0)
		{
			m_RecoilModifiers[0] = recoilModifier;
			m_RecoilModifiers[1] = recoilModifier;
			m_RecoilModifiers[2] = recoilModifier;
		}
		
		if (swayModifier > 0.0)
		{
			m_SwayModifiers[0] = swayModifier;
			m_SwayModifiers[1] = swayModifier;
			m_SwayModifiers[2] = swaySpeedModifier;
		}
		
		ApplyDeltaForceActiveLaserModifiers(weapon);
		
		DeltaForceGunplayConfig config = DeltaForceGunplayConfig.GetInstance();
		if (config && config.DebugMode)
		{
			Print("[DeltaForce Gunplay] Applied weapon property modifiers: " + weapon.GetType() + " recoil=" + recoilModifier + " sway=" + swayModifier);
		}
	}
	
	void ApplyDeltaForceActiveLaserModifiers(Weapon_Base weapon)
	{
		if (!weapon || !weapon.GetInventory()) return;
		
		for (int i = 0; i < weapon.GetInventory().AttachmentCount(); i++)
		{
			EntityAI attachment = weapon.GetInventory().GetAttachmentFromIndex(i);
			ApplyDeltaForceActiveLaserModifier(attachment);
			
			if (!attachment || !attachment.GetInventory()) continue;
			
			for (int j = 0; j < attachment.GetInventory().AttachmentCount(); j++)
			{
				EntityAI subattachment = attachment.GetInventory().GetAttachmentFromIndex(j);
				ApplyDeltaForceActiveLaserModifier(subattachment);
			}
		}
	}
	
	void ApplyDeltaForceActiveLaserModifier(EntityAI attachment)
	{
		if (!attachment) return;
		
		string attachmentType = attachment.GetType();
		if (!attachmentType.Contains("My_DF_Weapons_Attachments_laser"))
		{
			return;
		}
		
		ItemBase item = ItemBase.Cast(attachment);
		if (!item || !item.GetCompEM() || !item.GetCompEM().IsWorking())
		{
			return;
		}
		
		m_RecoilModifiers[0] = m_RecoilModifiers[0] * 0.8;
		m_RecoilModifiers[1] = m_RecoilModifiers[1] * 0.8;
		m_RecoilModifiers[2] = m_RecoilModifiers[2] * 0.8;
		
		m_SwayModifiers[0] = m_SwayModifiers[0] * 0.8;
		m_SwayModifiers[1] = m_SwayModifiers[1] * 0.8;
		m_SwayModifiers[2] = m_SwayModifiers[2] * 0.8;
		
		m_SightMisalignment[0] = m_SightMisalignment[0] * 0.8;
		m_SightMisalignment[1] = m_SightMisalignment[1] * 0.8;
		m_SightMisalignment[2] = m_SightMisalignment[2] * 0.8;
	}
}
