#include "../include/ImGuiHeirarchyPane.hpp"
#include "../include/Scene.hpp"
#include "../include/Logger.hpp"

namespace FF {
ImGuiHeirarchyPane::ImGuiHeirarchyPane(FF::Scene2& scene):
  ImGuiPane("Heirarchy"), scene(scene) {
  entt::entity e = scene.NewEntity("Entity0");
  // scene.GetComponent<Transform>(e)->scale = glm::vec3(50, 50, 50);
  scene.Traverse();
}

ImGuiHeirarchyPane::~ImGuiHeirarchyPane() {}

void ImGuiHeirarchyPane::Show(FF::Window& window) {
  if (!visible)
    return;
  ImGui::Begin(name.c_str());
  if (ImGui::Button("New Entity")) {
    scene.NewEntity("Entity");
  }
  ShowHeirarchy(scene.scene_root);
  
  
  // NOTE: The entities aren't actually deleted here, so there ARE memory leaks
  ImGui::End();
}

void ImGuiHeirarchyPane::ShowHeirarchy(entt::entity root) {
  if (root == entt::null)
    return;
  int result = ShowEntityNode(root);
  if (result != 0) {
    Relationship* r = scene.GetComponent<Relationship>(root);
    for (int i = 0; i < r->children.size(); i++) {
      ShowHeirarchy(r->children.at(i));
    }
    ImGui::TreePop();
  }
}

// 0 = tree node closed
// 1 = tree node open
// 2 = tree node deleted
int ImGuiHeirarchyPane::ShowEntityNode(entt::entity node) {
  // std::cout << node << std::endl;
  // if (node->is_dirty)
  //   return false;
  
  // I can be confident that entities will have Identifiers
  Identifier* p_id = scene.GetComponent<Identifier>(node);
  std::string node_id = p_id->id;
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
  flags |= main_selection_id == node_id ? ImGuiTreeNodeFlags_Selected : 0; 

  bool opened = ImGui::TreeNodeEx(node_id.c_str(), flags);
  if (ImGui::IsItemClicked(0) && node_id != "root") {
    main_selection_id = node_id;
    scene.selection = node;
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
    }
    ImGui::EndPopup();
  }
  
  if (ImGui::BeginDragDropSource()) {
    ImGui::SetDragDropPayload("ENTITY_MOVE_PAYLOAD", &node, sizeof(entt::entity));
    ImGui::EndDragDropSource();
  }
  /*
  root
  \_ Entity0
     \_ Entity1
        \_ Entity3
  \_ Entity2
  */

  /*
    TODO: Check if the entity begin dragged TO, is a children of the node being dragged FROM
  */
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_MOVE_PAYLOAD")) {

      // node is TO
      // e    is FROM
      
      entt::entity e = *(entt::entity*) payload->Data;
      FF_LOG_INFO("{}", (void*)&e);
      FF_LOG_INFO("{}", (void*)&node);
      if (!scene.IsChildOfRec(node, e) && e != node) {
        scene.MoveEntity(e, node);
      }
    }
    ImGui::EndDragDropTarget();
  }
  // do other things
  return opened;
}
}
