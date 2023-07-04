modded class ActionCraftBoneKnife: ActionContinuousBase
{
override void OnFinishProgressServer( ActionData action_data )
{
	
		super.OnFinishProgressServer(action_data);
        action_data.m_Player.AddExperienceOnCraftTool();
}
};