modded class CraftBoneHook extends RecipeBase 
{
    override void Do(ItemBase ingredients[], PlayerBase player,array<ItemBase> results, float specialty_weight)//gets called upon recipe's completion
	{
		super.Do(ingredients, player, results, specialty_weight);
        player.AddExperienceOnCraftTool();
	}
}