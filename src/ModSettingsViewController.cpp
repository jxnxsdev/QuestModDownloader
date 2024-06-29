#include "ModSettingsViewController.hpp"
#include "main.hpp"

#include "HMUI/Touchable.hpp"
#include "bsml/shared/BSML-Lite.hpp"
#include "UnityEngine/Application.hpp"

using namespace BSML::Lite;
using namespace HMUI;

void DidActivate(ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        self->get_gameObject()->AddComponent<Touchable*>();
        UnityEngine::Application::set_runInBackground(true);

        // url https://127.0.0.1:2777
        auto icall = il2cpp_utils::resolve_icall<void, StringW>("UnityEngine.Application::OpenURL");
        icall("http://127.0.0.1:2777/");
    }
}