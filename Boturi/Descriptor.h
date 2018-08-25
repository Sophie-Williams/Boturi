#pragma once
class Descriptor
{
private:
	VkDescriptorSetLayout layout;
	VkDescriptorPool pool;
	std::vector<VkDescriptorSet> sets;

	VkResult makeDescriptorSetLayout(std::vector<BindingType> definition);
	VkResult makeDescriptorPool(std::vector<BindingType> definition);
	VkResult makeDescriptorSets(std::vector<BindingType> definition);
public:
	Descriptor();

	Descriptor(std::vector<BindingType> definition);

	void cleanup();
	VkDescriptorSetLayout getLayout();
};