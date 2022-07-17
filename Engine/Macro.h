#pragma once

#define OUT
#define IN

/* ------------------------------------------------
                    Mapping
------------------------------------------------ */

#define DEVICE				        GEngine->GetDevice()->GetDevice()
#define GRAPHICS_CMD_LIST	        GEngine->GetGraphicsCmdQueue()->GetGraphicsCmdList()
#define RESOURCE_CMD_LIST	        GEngine->GetGraphicsCmdQueue()->GetResourceCmdList()
#define COMPUTE_CMD_LIST	        GEngine->GetComputeCmdQueue()->GetComputeCmdList()

#define GRAPHICS_ROOT_SIGNATURE		GEngine->GetRootSignature()->GetGraphicsSignature()
#define COMPUTE_ROOT_SIGNATURE		GEngine->GetRootSignature()->GetComputeSignature()

#define CONST_BUFFER(type)	        GEngine->GetConstantBuffer(type)

#define DELTATIME			        GET_SINGLE(Timer)->GetDeltaTime()
#define INPUT				        GET_SINGLE(Input)
#define EDITOR						GET_SINGLE(EditorManager)

#define BASE_PATH_KEY               "BasePath"
#define TEXTURE_PATH_KEY            "Texture"
#define SHADER_PATH_KEY             "Shader"
#define FBX_PATH_KEY                "FBX"
#define SCENE_PATH_KEY              "Scene"
#define SOUND_PATH_KEY					"Sound"

#define TEXTURE_PATH                GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH_KEY)
#define SHADER_PATH                 GET_SINGLE(PathManager)->FindPath(SHADER_PATH_KEY)
#define FBX_PATH                    GET_SINGLE(PathManager)->FindPath(FBX_PATH_KEY)
#define SCENE_PATH                  GET_SINGLE(PathManager)->FindPath(SCENE_PATH_KEY)

#ifdef _DEBUG
#define FILE_EXPENSION             L".txt"
#else
#define FILE_EXPENSION             L".json"
#endif

/* ------------------------------------------------
                    SingleTon
------------------------------------------------ */



#define DECLARE_SINGLE(type)					\
private:										\
	type() {}									\
	~type() {}									\
public:											\
	static type* GetI()							\
	{											\
		static type instance;					\
		return &instance;						\
	}											\



#define GET_SINGLE(type)		type::GetI()

#define WEAK(T) T.lock()

/* ------------------------------------------------
                    Editor 전용
------------------------------------------------ */


#define MENUITEM(Title, Class, Func)					\
inline static struct Register_##Class_##Func			\
{														\
	Register_##Class_##Func()							\
	{													\
		EDITOR->RegisterMenuBar(Title, Class::Func);	\
	}													\
} Register_##Class_##Func	

#define MENUITEM_PRESENT(Title, Class, Func)				\
inline static struct Register_##Class_##Func				\
{															\
	Register_##Class_##Func()								\
	{														\
		EDITOR->RegisterMenuBar(Title, Class::Func);		\
		Class::m_pInstance = EDITOR->GetWindow<Class>();	\
	}														\
} Register_##Class_##Func	



/* ------------------------------------------------
                    PROPERTY
------------------------------------------------ */


#define G_PROPERTY(_get) _declspec(property(get = _get))
#define S_PROPERTY(_set) _declspec(property(set = _set))

#define SETEVENTMEMBER(type, member, Event)									\
public:																		\
	void Set##member(const type& p_val) { m_##member = p_val; Event(); }     


#define PROPERTY(_get, _set) _declspec(property(get = _get, put = _set))

#define GETMEMBER(type, member)										\
public:																\
	type& Get##member() { return m_##member; }			

#define SETMEMBER(type, member)										\
public:																\
	void Set##member(const type& p_val) { m_##member = p_val; }     

#define PRIVATE_PROPERTY(type, member)								\
public:                                                             \
	SETMEMBER(type, member)											\
	GETMEMBER(type, member)											\
	PROPERTY(Get##member,  Set##member) type& ##member;				\
private:                                                            \
	type m_##member         


/* ----- 김치삼 교수님께서 알려주신 PROPERTY (개조 버전) ----- */

#define PUBLIC_PROPERTY(type, member)								\
public:                                                             \
	SETMEMBER(type, member)											\
	GETMEMBER(type, member)											\
	PROPERTY(Get##member, Set##member) type& ##member;				\
public:                                                             \
	type m_##member                                                                                        

#define PROTECTED_PROPERTY(type, member)							\
public:                                                             \
	SETMEMBER(type, member)											\
	GETMEMBER(type, member)											\
	PROPERTY(Get##member,  Set##member) type& ##member;				\
protected:                                                          \
	type m_##member                       


#define PRIVATE_SET_EVENT_PROPERTY(type, member, Event)						\
public:                                                             \
	SETEVENTMEMBER(type, member, Event)								\
	GETMEMBER(type, member)											\
	PROPERTY(Get##member,  Set##member) type& ##member;				\
protected:                                                          \
	type m_##member                       

#define PROTECTED_SET_EVENT_PROPERTY(type, member, Event)						\
public:                                                             \
	SETEVENTMEMBER(type, member, Event)								\
	GETMEMBER(type, member)											\
	PROPERTY(Get##member,  Set##member) type& ##member;				\
protected:                                                          \
	type m_##member                       


/*	--------------------------------------------
			EnumClass To String
-------------------------------------------- */

/* ------ 제공 : Utils Forever ------ */
// https://github.com/utilForever/CubbyEnums/blob/master/Fruit.hpp

enum class LAYER_TYPE : uint32
{
#define X(a) a,
#include "layer.hpp"
#undef X
};

const std::string LAYER_TYPE_STR[] = {
#define X(a) #a,
#include "layer.hpp"
#undef X
};

template <class T>
T StrToEnum(std::string);
template <class T>
std::string EnumToStr(T);

#define STR2ENUM(TYPE, ARRAY)                                                \
    template <>                                                              \
    inline TYPE StrToEnum<TYPE>(std::string str)                             \
    {                                                                        \
        for (std::size_t i = 0; i < (sizeof(ARRAY) / sizeof(ARRAY[0])); ++i) \
        {                                                                    \
            if (ARRAY[i] == str)                                             \
            {                                                                \
                return TYPE(i);                                              \
            }                                                                \
        }                                                                    \
                                                                             \
        return TYPE(0);                                                      \
    }

#define ENUM2STR(TYPE, ARRAY)                   \
    template <>                                 \
    inline std::string EnumToStr<TYPE>(TYPE v)  \
    {                                           \
        return ARRAY[static_cast<int>(v)];      \
    }

#define ENUM_AND_STR(TYPE, ARRAY)               \
    STR2ENUM(TYPE, ARRAY)                       \
    ENUM2STR(TYPE, ARRAY)

ENUM_AND_STR(LAYER_TYPE, LAYER_TYPE_STR)        


/*	--------------------------------------------
					Crash
-------------------------------------------- */

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(expr)			\
{									\
	if (!(expr))					\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(expr);	\
	}								\
}

#define SAFEDELETE(ptr)		\
if (ptr) {					\
		delete ptr;			\
}							\
ptr = nullptr			