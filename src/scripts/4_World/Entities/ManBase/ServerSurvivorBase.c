modded class SurvivorBase
{
	void DoSkinning(PlayerBase butcher, ItemBase item)
	{
		if (m_alreadySkinned)
			return;
		
		vector body_pos = GetPosition();
		
		ItemBase added_item;
		float meatCountMod = butcher.GetPerkFloatValue(SyberiaPerkType.SYBPERK_HUNTING_MEAT_COUNT, 0, 0);
		float skinningMod = butcher.GetPerkFloatValue(SyberiaPerkType.SYBPERK_HUNTING_SKINING, 0, 0);
		string skinningCfg = CFG_VEHICLESPATH + " " + GetType() + " Skinning";
		int skinningChildsCount = GetGame().ConfigGetChildrenCount(skinningCfg);
		string itemName;
		float count;
		float quantityMin, quantityMax;
		string skinningChildName;
		
		for (int childId = 0; childId < skinningChildsCount; childId++)
		{
			if (GetGame().ConfigGetChildName(skinningCfg, childId, skinningChildName))
			{
				itemName = GetGame().ConfigGetTextOut(skinningCfg + " " + skinningChildName + " item");
				count = GetGame().ConfigGetFloat(skinningCfg + " " + skinningChildName + " count");
				
				vector pos_rnd = body_pos + Vector(Math.RandomFloat01() - 0.5, 0, Math.RandomFloat01() - 0.5);
		
				if (skinningChildName == "Steaks")
				{
					if (count > 0) 
					{
						quantityMin = Math.Clamp(0.2 + meatCountMod, 0.2, 0.9);
						quantityMax = Math.Clamp(0.3 + meatCountMod, 0.3, 1.0);			
						count = Math.Max((count * (meatCountMod + 0.2)) + 1, 1);
						
						while (count > 0) {
							count = count - 1.0;
							added_item = ItemBase.Cast(GetGame().CreateObjectEx( itemName, pos_rnd, ECE_PLACE_ON_SURFACE ));
							if (added_item) {
								added_item.SetQuantity(Math.Round(Math.RandomFloat(quantityMin, quantityMax) * added_item.GetQuantityMax()), false);
								added_item.InsertAgent(eAgents.SALMONELLA, 1);
								added_item.SetTemperature(38);
							}
						}
					}
				}
				else if (skinningChildName == "Lard")
				{
					if (meatCountMod > 0.3 && count > 0) 
					{
						quantityMin = Math.Clamp(0.1 + meatCountMod, 0.1, 0.9);
						quantityMax = Math.Clamp(0.3 + meatCountMod, 0.3, 1.0);
						
						while (count > 0) {
							count = count - 1.0;
							added_item = ItemBase.Cast(GetGame().CreateObjectEx( itemName, pos_rnd, ECE_PLACE_ON_SURFACE ));
							if (added_item) {
								added_item.SetQuantity(Math.Round(Math.RandomFloat(quantityMin, quantityMax) * added_item.GetQuantityMax()), false);
								added_item.InsertAgent(eAgents.SALMONELLA, 1);
								added_item.SetTemperature(38);
							}
						}
					}
				}
				else if (skinningChildName == "Bones")
				{
					if (count > 0) 
					{	
						count = Math.Max((count * (meatCountMod + 0.5)), 1);
						quantityMin = Math.Clamp(0.2 + meatCountMod, 0.2, 0.9);
						quantityMax = Math.Clamp(0.3 + meatCountMod, 0.3, 1.0);
						
						while (count > 0) {
							count = count - 1.0;
							added_item = ItemBase.Cast(GetGame().CreateObjectEx( itemName, pos_rnd, ECE_PLACE_ON_SURFACE ));
							if (added_item) {
								added_item.SetQuantity(Math.Round(Math.RandomFloat(quantityMin, quantityMax) * added_item.GetQuantityMax()), false);
								added_item.InsertAgent(eAgents.SALMONELLA, 1);
								added_item.SetTemperature(38);
								added_item.SetHealth01("", "", meatCountMod);
							}
						}
					}
				}
				else if (skinningChildName == "Guts")
				{
					if (count > 0) 
					{
						quantityMin = Math.Clamp(0.2 + meatCountMod, 0.2, 0.9);
						quantityMax = Math.Clamp(0.3 + meatCountMod, 0.3, 1.0);
		
						while (count > 0) {
							count = count - 1.0;
							added_item = ItemBase.Cast(GetGame().CreateObjectEx( itemName, pos_rnd, ECE_PLACE_ON_SURFACE ));
							if (added_item) {
								added_item.SetQuantity(Math.Round(Math.RandomFloat(quantityMin, quantityMax) * added_item.GetQuantityMax()), false);
								added_item.InsertAgent(eAgents.SALMONELLA, 1);
								added_item.SetTemperature(38);
								added_item.SetHealth01("", "", meatCountMod);
							}
						}
					}
				}
				else
				{
					while (count > 0) {
						count = count - 1.0;
						ItemBase.Cast(GetGame().CreateObjectEx( itemName, pos_rnd, ECE_PLACE_ON_SURFACE ));
					}
				}
			}
		}
		
		butcher.AddExperience(SyberiaSkillType.SYBSKILL_HUNTING, GetSyberiaConfig().m_skillsExpHuntingButchCommon);
		
		m_alreadySkinned = true;
		
		ItemBase gloves = butcher.GetItemOnSlot("Gloves");
		if (gloves)
		{
			gloves.SetCleanness(0);
		}
		else
		{
			butcher.SetBloodyHands(true);
		}
	}
};