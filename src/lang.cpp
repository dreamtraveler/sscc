#include <unordered_map>
#include <memory>
#include "lang.h"
#include "log.h"
static std::unordered_map<const char*, std::shared_ptr<Language>> __map;

Language::Language(const char *name) : _name(Unistr::get(name, strlen(name)))
{ }

Language *Language::get(const char *name) {
	name = Unistr::get(name, strlen(name));
    auto it = __map.find(name);
    if (it == __map.end()) {
        return nullptr;
    }
    return it->second.get();
}

Language *Language::reg(std::shared_ptr<Language> lang) {
    auto em = __map.emplace(lang->name(), lang);
    return em.first->second.get(); 
}



