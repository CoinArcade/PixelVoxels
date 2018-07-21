#include "Transform.h"

//Can't exist without being an component from an entity
//Defines the local transform of an entity
typedef struct Transform{
    Vector3 position;
    Matrix3x3 rotation;
}Transform;

//Function that returns the ID of this component
static ComponentID ThisComponentID(){
    static ComponentID CompID = -1;
    if(CompID<0)
        CompID = GetComponentID("Transform");
    
    return CompID;
}

extern engineECS ECS;
extern engineCore Core;

void TransformConstructor(void** data){
    if(!data) return;
    *data = calloc(1,sizeof(Transform));
    ((Transform*)*data)->rotation = EulerAnglesToMatrix3x3(VECTOR3_ZERO);
}

void TransformDestructor(void** data){
    if(!data) return;
    free(*data);
    *data = NULL;
}

void* TransformCopy(void* data){
    if(!data) return NULL;
    Transform *newTransform = malloc(sizeof(Transform));
    memcpy(newTransform,data,sizeof(Transform));
	return newTransform;
}

cJSON* TransformEncode(void** data, cJSON* currentData){
    if(!data) return NULL;
    Transform *tr = *data; 

    Vector3 rot = Matrix3x3ToEulerAngles(tr->rotation);

    int hasChanged = 0;
    if(currentData){
        //Check if any data has changed
        cJSON *curPos = cJSON_GetObjectItem(currentData,"position");
        cJSON *curRot = cJSON_GetObjectItem(currentData,"rotation");
        if(tr->position.x != (cJSON_GetArrayItem(curPos,0))->valuedouble ||
           tr->position.y != (cJSON_GetArrayItem(curPos,1))->valuedouble || 
           tr->position.z != (cJSON_GetArrayItem(curPos,2))->valuedouble || 
           rot.x != (cJSON_GetArrayItem(curRot,0))->valuedouble || 
           rot.y != (cJSON_GetArrayItem(curRot,1))->valuedouble || 
           rot.z != (cJSON_GetArrayItem(curRot,2))->valuedouble
        ){
            hasChanged = 1;
        }
    }

    //Encode this component if its not from a prefab (who has currentData) or if it has changed
    if(!currentData || hasChanged){
        cJSON *obj = cJSON_CreateObject();

        cJSON *position = cJSON_AddArrayToObject(obj,"position");
        cJSON_AddItemToArray(position, cJSON_CreateNumber(tr->position.x));
        cJSON_AddItemToArray(position, cJSON_CreateNumber(tr->position.y));
        cJSON_AddItemToArray(position, cJSON_CreateNumber(tr->position.z));

        cJSON *rotation = cJSON_AddArrayToObject(obj,"rotation");
        cJSON_AddItemToArray(rotation, cJSON_CreateNumber(rot.x));
        cJSON_AddItemToArray(rotation, cJSON_CreateNumber(rot.y));
        cJSON_AddItemToArray(rotation, cJSON_CreateNumber(rot.z));

        return obj;
    }
    return NULL;
}

void* TransformDecode(cJSON **data){
    Transform *tr = malloc(sizeof(Transform));
    cJSON *pos = cJSON_GetObjectItem(*data,"position");
    tr->position = (Vector3){(cJSON_GetArrayItem(pos,0))->valuedouble,
                             (cJSON_GetArrayItem(pos,1))->valuedouble,
                             (cJSON_GetArrayItem(pos,2))->valuedouble};

    cJSON *rot = cJSON_GetObjectItem(*data,"rotation");
    tr->rotation = EulerAnglesToMatrix3x3((Vector3){(cJSON_GetArrayItem(rot,0))->valuedouble,
                                                    (cJSON_GetArrayItem(rot,1))->valuedouble,
                                                    (cJSON_GetArrayItem(rot,2))->valuedouble});

    return tr;
}

Vector3 GetPosition(EntityID entity){
    if(!EntityContainsComponent(entity, ThisComponentID())){
        PrintLog(Warning,"GetPosition: Entity doesn't have a Transform component. (%d)\n",entity);
        return VECTOR3_ZERO;
    }
    Transform *transform = (Transform *)ECS.Components[ThisComponentID()][entity].data;
    return transform->position;
}

Vector3 GetRotation(EntityID entity){
    if(!EntityContainsComponent(entity, ThisComponentID())){
        PrintLog(Warning,"GetRotation: Entity doesn't have a Transform component. (%d)\n",entity);
        return VECTOR3_ZERO;
    }
    Transform *transform = (Transform *)ECS.Components[ThisComponentID()][entity].data;
    return Matrix3x3ToEulerAngles(transform->rotation);
}

Matrix3x3 GetRotationMatrix(EntityID entity){
    if(!EntityContainsComponent(entity, ThisComponentID())){
        PrintLog(Warning,"GetRotationMatrix: Entity doesn't have a Transform component. (%d)\n",entity);
        return Identity();
    }
    Transform *transform = (Transform *)ECS.Components[ThisComponentID()][entity].data;
    return transform->rotation;
}

void SetPosition(EntityID entity, Vector3 pos){
    if(!EntityContainsComponent(entity, ThisComponentID())){
        PrintLog(Warning,"SetPosition: Entity doesn't have a Transform component. (%d)\n",entity);
        return;
    }
    Transform *transform = (Transform *)ECS.Components[ThisComponentID()][entity].data;
    transform->position = pos;
}

void SetRotation(EntityID entity, Vector3 rot){
    if(!EntityContainsComponent(entity, ThisComponentID())){
        PrintLog(Warning,"SetRotation: Entity doesn't have a Transform component. (%d)\n",entity);
        return;
    }
    Transform *transform = (Transform *)ECS.Components[ThisComponentID()][entity].data;
    transform->rotation = EulerAnglesToMatrix3x3(rot);
}

void GetGlobalTransform(EntityID entity, Vector3 *outPos, Vector3 *outEuler, Matrix3x3 *outRot){
    if(!EntityContainsComponent(entity, ThisComponentID())){
        PrintLog(Warning,"GetGlobalTransform: Entity doesn't have a Transform component. (%d)\n",entity);
        return;
    }
    Transform *transform = (Transform *)ECS.Components[ThisComponentID()][entity].data;

    if(EntityIsChild(entity)){
        Vector3 parentGlobalPos, parentGlobalEuler;
        Matrix3x3 parentGlobalRot;
        GetGlobalTransform(GetEntityParent(entity),&parentGlobalPos, &parentGlobalEuler, &parentGlobalRot);

        //Apply rotation matrix
        Vector3 p = (Vector3){transform->position.x, transform->position.y, transform->position.z};
        Vector3 rotatedPos = RotateVector(p,parentGlobalRot);

        rotatedPos = (Vector3){ rotatedPos.x + parentGlobalPos.x, rotatedPos.y + parentGlobalPos.y, rotatedPos.z + parentGlobalPos.z};

        Matrix3x3 rotatedRot = MultiplyMatrix3x3(transform->rotation, parentGlobalRot);

        if(outPos) *outPos = rotatedPos;
        if(outRot) *outRot = rotatedRot;
        if(outEuler) *outEuler = Matrix3x3ToEulerAngles(rotatedRot);
        
    }else{
        if(outPos) *outPos = transform->position;
        if(outRot) *outRot = transform->rotation;
        if(outEuler) *outEuler = Matrix3x3ToEulerAngles(transform->rotation);
    }
}

static int l_SetPosition (lua_State *L) {
    //Get the arguments
    EntityID id = luaL_checkinteger (L, 1);
    if(!lua_istable(L, 2)){
        PrintLog(Warning,"SetPosition(Lua): Second argument must be a table with 'x', 'y' and 'z' numbers!\n");
        luaL_checktype(L, 2, LUA_TTABLE); //Check again to cause script error and stop execution
        return 0;
    }
    lua_getfield(L,2, "x");
    lua_getfield(L,2, "y");
    lua_getfield(L,2, "z");

    Vector3 pos = {luaL_checknumber(L,-3), luaL_checknumber(L,-2), luaL_checknumber(L,-1)};

    SetPosition(id, pos);
    lua_pop(L, 3);
    return 0; //Return number of results
}

static int l_SetRotation (lua_State *L) {
    //Get the arguments
    EntityID id = luaL_checkinteger (L, 1);
    if(!lua_istable(L, 2)){
        PrintLog(Warning,"SetRotation(Lua): Second argument must be a table with 'x', 'y' and 'z' numbers!\n");
        luaL_checktype(L, 2, LUA_TTABLE); //Check again to cause script error and stop execution
        return 0;
    }
    lua_getfield(L,2, "x");
    lua_getfield(L,2, "y");
    lua_getfield(L,2, "z");

    Vector3 rot = {luaL_checknumber(L,-3), luaL_checknumber(L,-2), luaL_checknumber(L,-1)};

    SetRotation(id, rot);
    lua_pop(L, 3);
    return 0; //Return number of results
}

static int l_GetPosition (lua_State *L) {
    lua_settop(L, 1);
    EntityID id = luaL_checkinteger (L, 1); //Get the argument
    Vector3 pos = GetPosition(id);
    Vector3ToTable(L, pos); //Create return table and store the values
    return 1; //Return number of results
}

static int l_GetRotation (lua_State *L) {
    lua_settop(L, 1);
    EntityID id = luaL_checkinteger (L, 1); //Get the argument
    Vector3 pos = GetRotation(id);
    Vector3ToTable(L, pos); //Create return table and store the values
    return 1; //Return number of results
}

static int l_GetGlobalTransform (lua_State *L) {
    lua_settop(L, 1);
    EntityID id = luaL_checkinteger (L, 1); //Get the argument
    Vector3 pos, rot;
    GetGlobalTransform(id, &pos, &rot,NULL);

    Vector3ToTable(L, pos); //Create return tables and store the values
    Vector3ToTable(L, rot);
    return 2; //Return number of results
}


void TransformRegisterLuaFunctions(){
    lua_pushcfunction(Core.lua, l_SetPosition);
    lua_setglobal(Core.lua, "SetPosition");

    lua_pushcfunction(Core.lua, l_SetRotation);
    lua_setglobal(Core.lua, "SetRotation");

    lua_pushcfunction(Core.lua, l_GetPosition);
    lua_setglobal(Core.lua, "GetPosition");

    lua_pushcfunction(Core.lua, l_GetRotation);
    lua_setglobal(Core.lua, "GetRotation");

    lua_pushcfunction(Core.lua, l_GetGlobalTransform);
    lua_setglobal(Core.lua, "GetGlobalTransform");
}