#include "../include/ImGuiHeirarchyPane.hpp"
#include "../include/Scene.hpp"
#include "../include/Logger.hpp"

namespace FF {
ImGuiHeirarchyPane::ImGuiHeirarchyPane(FF::Scene& scene):
  ImGuiPane("Heirarchy"), scene(scene) {
  scene.NewEntity("Entity0");
}

ImGuiHeirarchyPane::~ImGuiHeirarchyPane() {}

void ImGuiHeirarchyPane::Show(FF::Window& window) {
  if (!visible)
    return;
  ImGui::Begin(name.c_str());
  if (ImGui::Button("New Entity")) {
    scene.NewEntity("Entity" + std::to_string(scene.GetEntityCount()));
  }
  // ValidateTree(scene.selected_entity);
  // FF_LOG_INFO("Validated tree");
  ShowHeirarchy(scene.entity_tree_root);
  
  
  // NOTE: The entities aren't actually deleted here, so there ARE memory leaks
  ImGui::End();
}

void ImGuiHeirarchyPane::ShowHeirarchy(FF::Entity* root) {
  // traverse tree and render it
  if (root == nullptr)
    return;
  int result = ShowEntityNode(root);
  // std::cout << root->is_dirty << std::endl;
  if (result != 0) {
    for (int i = 0; i < root->children.size(); i++) {
      ShowHeirarchy(root->children.at(i));
    }
    ImGui::TreePop();
  }
}

// 0 = tree node closed
// 1 = tree node open
// 2 = tree node deleted
int ImGuiHeirarchyPane::ShowEntityNode(FF::Entity* node) {
  // std::cout << node << std::endl;
  if (node->is_dirty)
    return false;
  
  // I can be confident that entities will have Identifiers
  std::string node_id = node->GetComponent<Identifier>()->id;
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
  flags |= main_selection_id == node_id ? ImGuiTreeNodeFlags_Selected : 0; 

  bool opened = ImGui::TreeNodeEx(node->GetComponent<Identifier>()->id.c_str(), flags);
  if (ImGui::IsItemClicked(0) && node_id != "root") {
    main_selection_id = node_id;
    scene.selected_entity = node;
  }
  if (ImGui::IsItemClicked(1) && node_id != "root") {
    ImGui::OpenPopup("RightClickEntityMenu");
    right_click_selection_id = node_id;
  }
  if (ImGui::BeginPopup("RightClickEntityMenu")) {
    if (right_click_selection_id == node_id && ImGui::Button("Delete Entity")) {
      /*
        NOTE: While the entity isn't deleted here, it IS guarunteed to be freed when the program ends
        Future plans to implement a free list for entity reuse
      */
      node->MarkDirty(true);
    }
    ImGui::EndPopup();
  }
  
  if (ImGui::BeginDragDropSource()) {
    ImGui::SetDragDropPayload("ENTITY_MOVE_PAYLOAD", &node, sizeof(FF::Entity*));
    ImGui::EndDragDropSource();
  }
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_MOVE_PAYLOAD")) {
      FF::Entity* e = *(FF::Entity**) payload->Data;
      if (e != node) {
        node->AddChild(e);
      }
    }
    ImGui::EndDragDropTarget();
  }
  // do other things
  return opened;
}

void ImGuiHeirarchyPane::ValidateTree(FF::Entity* root) {
  if (root == nullptr) {
    return;
  }
  if (root != nullptr && root->is_dirty) {
    FF_LOG_INFO("Entity '{}' marked dirty, deleting it", (void*)root);
    scene.DeleteEntity(root);
    FF_LOG_INFO("Entity fully deleted {}", (void*)root);
    return;
  }
  for (int i = 0; i < root->children.size(); i++) {
    ValidateTree(root->children.at(i));
  }
}
}
