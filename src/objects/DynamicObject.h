#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include <vector>
#include <irrlicht.h>
#include "properties.h"
#include "projectileItem.h" //projectile data
#include "HealthSceneNode.h"
#include "../sound/SoundManager.h"

using namespace std; // for the vector class
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

extern "C" { // Access to the LUA library
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

//Note, also inherit from cproperty, but this in already used in projectile items (they both share properties)
class DynamicObject : public projectileitem{
    public:

		typedef struct data_anim{
			stringc name;
			stringc stance;
			stringc wear;
			u32 variation;
			stringc meshname;
			IAnimatedMesh* mesh;
			stringc sound;
			s32 startFrame;
			s32 endFrame;
			vector<s32> attackevent;
			vector<s32> soundevent;
			f32 speed;
			f32 walkspeed;
			bool loop;

		}DynamicObject_Animation;

		typedef struct data_attachment{
			core::stringw name;
			core::stringw bonename;
			core::vector3df attachpos;
			core::vector3df attachrot;
			DynamicObject* currentlyAttached;
		}DynamicObject_attachment;

		typedef struct data_material{
			u32 id;
			stringc shader;
			stringc texture0;
			stringc texture1;
			stringc texture2;
			stringc texture3;

		}DynamicObject_material;

		enum TYPE
		{
			OBJECT_TYPE_NONE = 0,
			OBJECT_TYPE_NPC = 1,
			OBJECT_TYPE_INTERACTIVE = 2,
			OBJECT_TYPE_NON_INTERACTIVE = 3,
			OBJECT_TYPE_WALKABLE = 4,
			OBJECT_TYPE_PLAYER	= 5,
			OBJECT_TYPE_EDITOR	= 6,
			OBJECT_TYPE_LOOT = 7,	
			OBJECT_TYPE_ALL
		};

		enum OBJECT_ANIMATION
		{
			OBJECT_ANIMATION_IDLE = 0,
			OBJECT_ANIMATION_WALK = 1,
			OBJECT_ANIMATION_RUN = 2,
			OBJECT_ANIMATION_ATTACK = 3,
			OBJECT_ANIMATION_INJURED = 4,
			OBJECT_ANIMATION_KNOCKBACK = 5,
			OBJECT_ANIMATION_DIE = 6,
			OBJECT_ANIMATION_DIE_KNOCKBACK = 7,
			OBJECT_ANIMATION_SPAWN = 7,
			OBJECT_ANIMATION_DESPAWN = 8,
			OBJECT_ANIMATION_DESPAWN_KNOCKBACK = 9,
			OBJECT_ANIMATION_OPEN = 10,
			OBJECT_ANIMATION_CLOSE = 11,
			OBJECT_ANIMATION_PRESPAWN = 12,
			OBJECT_ANIMATION_CUSTOM = 99
		};

		enum SPECIAL
		{
			SPECIAL_NONE = 0,
			SPECIAL_SEGMENT,
			SPECIAL_LOOT
		};

		enum AI_INFO
		{
			AI_STATE_IDLE = 0,
			AI_STATE_BUSY
		};

		DynamicObject(stringc name, stringc meshFile, vector<DynamicObject_Animation> animations, bool directpath = false);
		virtual ~DynamicObject();

        DynamicObject* clone();

		ISceneNode* getNode();
		ISceneNode* getShadow();
		stringc getTemplateObjectName();
        void setTemplateObjectName(stringc newName);

		void lookAt(vector3df pos);
		void rotateObject(vector3df from, vector3df to, u32 time); // Will rotate the object from->to in a specified time
		void setPosition(vector3df pos);
		void setOldPos();
        vector3df getPosition();
		void setRotation(vector3df rot);
        vector3df getRotation();
		void moveObject(f32 speed);
		void walkTo(vector3df targetPos);
		bool isWalking();
		f32 rayTest(vector3df pos, vector3df pos1);
		void setWalkTarget(vector3df newTarget);
		vector3df getWalkTarget();
		f32 getDistanceFrom(vector3df pos);

		f32 getObjectSize(bool withenemy=true);

		void setEnabled(bool enabled);
		bool isEnabled();
		void setType(stringc name);
		void setType(TYPE type);
		TYPE getType();

		void setName(stringc name);
        stringc getName();

		void setMaterialType(E_MATERIAL_TYPE mType);
        E_MATERIAL_TYPE getMaterialType();

		void setScale(vector3df scale);
		vector3df getScale();

		DynamicObject * getCurrentEnemy();
		void clearEnemy();
		cproperty initProperties();
		cproperty getProperties();
		void setProperties(cproperty prop);

		cproperty getProp_base();
		void setProp_base(cproperty prop);

		cproperty getProp_level();
		void setProp_level(cproperty prop);

		void setMaterials(vector<DynamicObject_material> mat);
		vector<DynamicObject_material> getMaterials();
		inline void setAttachment(vector<DynamicObject_attachment> attach) {attachments = attach;} 

		void setLife(int life);
        int getLife();
        void setMoney(int money);
        int getMoney();
		void setObjectLabel(stringc label);
        void objectLabelSetVisible(bool visible);

		void createTextAnim(core::stringw text=L"", video::SColor color=video::SColor(255,255,0,0), u32 duration=2000, dimension2d<f32> size=dimension2d<f32>(18,10));

		inline bool hasAnimation(){ return animations.size() != 0; };
        //void setAnimations( vector<DynamicObject_Animation> animations ) {this->animations = animations; };
        //vector<DynamicObject_Animation> getAnimations() {return this->animations;};
        void setFrameLoop(s32 start, s32 end);
        void setAnimationSpeed(f32 speed);
		DynamicObject::OBJECT_ANIMATION getAnimationState(stringc animName);
		DynamicObject::OBJECT_ANIMATION getAnimation(void);
        bool setAnimation(stringc animName);
		void checkAnimationEvent();
		void setRunningMode(bool run); // Switche the walk animation to a run animation

		void setAnimator(ISceneNodeAnimatorCollisionResponse* animator_node);
		ISceneNodeAnimatorCollisionResponse* getAnimator();
		ITriangleSelector* getTriangleSelector();

		// item management
        void addItem(stringc itemName);
        void removeItem(stringc itemName);
        vector<stringc> getItems();
        int getItemCount(stringc itemName);//returns the total of items of type "itemName"
        bool hasItem(stringc itemName);
        void removeAllItems();

		// loot management
		void addLootItem(DynamicObject* loot);
		//void addLoot(core::stringc templatename); //(add the loot directly in the dynamic object, create from template, parent and hide in this object)
		void removeLoot(DynamicObject* loot);
		void removeAllLoot();
		vector<DynamicObject*> getLootItems();

		inline vector<DynamicObject_attachment> getAttachments() {return attachments;}

		stringc getScript();
        void setScript(stringw script);
		void clearScripts();//delete lua_State
		void doScript();//called when the game starts
		void storeParams(); // Store the original position and rotation before gameplay (used when you start the game)
		void restoreParams();//restore original position and rotation after gameplay (used when you stop the game in Editor)
        void update();//run "step" lua function

        void notifyClick();
		void notifyAttackRange();
		void notifyCollision();
		void notifyAnswer(bool answer);
		void notifyUse(); //Implemented for the loot (potion etc.)
		void notifyWear(); //To implement for loot
		void notifyOnKeypressed(); // To warn that a key was pressed while playing

        stringc getObjectType();

		void attackEnemy(DynamicObject* obj);
		bool isTemplate();
		void setTemplate(bool value);
		inline void setTemplateScale(core::vector3df scale) { originalscale=scale; }
		inline void setThumbnail(core::stringc thumb){thumbnail=thumb; }
		inline core::stringc getThumbnail(){return thumbnail; }

		inline void setDescription(core::stringw desc){description=desc; }
		inline stringw getDescription(){return description; }
		inline void setSolid(bool solid){if (solid) getNode()->setID(100); else getNode()->setID(0);}


		// Needed in other classes
		DynamicObject_Animation currentAnim;
		AI_INFO AI_State;

		// DIRECT ACCESS VARIABLES WAs protected before
		stringc templateObjectName;//The original object name
		stringc fileName; // Information about the given filename 
		stringw script;  // The script of this object
		stringw displayName; // The display name
		stringw internalname; //Internal name

		cproperty prop_base;
		cproperty prop_level;
		cproperty properties;

		bool isEnemy; //Is the object an enemy of the player?
		bool isInBag; // Define if the object is stored in the inventory and not in the scene anymore
		bool isDestroyedAfterUse; //Tell if we remove the loot from the bag and "kill" it after use
		bool isGenerated; //Will tell if the object has been spawned directly ingame and not in the editor. 

		// projectile data
		projectileitem obj_projectile;


    protected:

        bool enabled;//disabled objects aren't rendered and step() function isn't processed during gameplay



        DynamicObject(stringc name, IAnimatedMesh* mesh, vector<DynamicObject_Animation> animations = vector<DynamicObject_Animation>());

    private:

		void setupObj(stringc name, IAnimatedMesh* mesh);

		void updateRotation(); // Update the rotation of the object based on refreshes
		void updateWalk(); // Update the walk movement based on refrehes
		void luaRefresh(); //Update the scripts

		//!Will splill the loot on the terrain. Called when the character dies
		void splillLoot();

        //lua funcs
		static int getEnemyCount(lua_State *ls);//Return the number of enemies
        static int setPosition(lua_State *ls);//setPosition(x,y,z)
        static int getPosition(lua_State *ls);//x,y,z = getPosition()
        static int setRotation(lua_State *ls);//setRotation(x,y,z)
        static int getRotation(lua_State *ls);//x,y,z = getPosition()

		static int getParentRotation(lua_State* LS); //Get the rotation of the parent node
		static int getParentPosition(lua_State* LS); //Get the position of the parent node

        static int turn(lua_State *ls);//turn(degrees)
        static int move(lua_State *ls);//move(x,y,z)
		static int walkToLUA(lua_State *ls); // walkToLua("objectName")
        static int lookAt(lua_State *ls);//lookAt(x,y,z)
        static int lookToObject(lua_State *ls);//lookToObject(otherObjectName)
		static int attackObj(lua_State *ls);//attackObj(otherObjectName)
		static int setProperty(lua_State *ls); // setPropertie("objectName","propertie",value)
		static int getProperty(lua_State *ls); // value getPropertie("objectName","propertie")
        static int distanceFrom(lua_State *ls);
		static int getNameLUA(lua_State *ls); // value getNameLUA()
		static int setNameLUA(lua_State *ls); // value getNameLUA()

        static int setFrameLoop(lua_State *ls);//setFrameLoop(start,end);
        static int setAnimation(lua_State *ls);//setAnimation(anim_name);
        static int setAnimationSpeed(lua_State *ls);//setAnimationSpeed(speed);

        static int showObjectLabel(lua_State *ls);//showObjectLabel()
        static int hideObjectLabel(lua_State *ls);//hideObjectLabel()
        static int setObjectLabel(lua_State *ls);//setObjectLabel(newLabelText)
		static int setEnemy(lua_State *ls);//Have C++ know this object is an enemy

		//Dialog Functions
        static int showDialogMessage(lua_State *ls);//showDialogMessage(text, optional_sound_file)
		static int showDialogQuestion(lua_State *ls);//showDialogQuestion(text, optional_sound_file)

        static int setEnabled(lua_State *ls);//setEnabled(enabled?)

		static int hasReached(lua_State *ls);// Check the status of the walk if reached the destination
		static int setObjectType(lua_State *ls); // change the object type
		static int addPlayerLoot(lua_State *ls); // add an object to the player loot
		static int addLootLUA(lua_State *ls); // Create an object from a template and add it to the object loot.

		static int destroyAfterUse(lua_State *ls); //Set the flag on the object if it must be removed after use (potions)
				
		static int isKeypressed(lua_State *ls); //Query what key was pressed
		static int shoot(lua_State *ls); //Shoot the currently defined projectile


		
        stringc name;

        IAnimatedMesh* mesh;
		IMesh* Tmesh; //Tangent mesh
        ISceneNode* node;
		IAnimatedMeshSceneNode * nodeAnim;
		ISkinnedMesh* skinnedmesh;

		ITriangleSelector* selector;
		ISceneNodeAnimatorCollisionResponse* animator;

        ISceneNode* fakeShadow;
		scene::HealthSceneNode* Healthbar;

		ISceneManager* smgr;
		IVideoDriver* driver;


		OBJECT_ANIMATION currentAnimation;
		OBJECT_ANIMATION oldAnimation;
		stringc oldAnimName;
		TYPE objectType;
		stringc typeText;

		DynamicObject* enemyUnderAttack;
		DynamicObject* currentObject;
		stringc namecollide;
		f32	currentSpeed;
		s32 lastframe;

		bool collided;
		bool nodeLuaCulling;
		bool deadstate;
		bool diePresent;
		bool despawnPresent;
		bool runningMode;
		bool reached; // check if reached the walktarget or not

		vector3df walkTarget;
		vector3df originalPosition;
		vector3df originalRotation;
		vector3df oldpos;
		ISceneNode* oldparent;

		// Used for timed rotation (fully implemented?) should be used to rotate things over time (doors, bridges, etc)
		// Could be used for the player to make it rotate at a speed limit (degrees per seconds)
		vector3df rotfrom;
		vector3df rotto;
		u32 rotationcounter;
		u32 rotationtime;
		bool rotationupdater; // check to update rotations or not

		int original_life;
		int original_maxlife;
		vector<stringc> items;
		vector<DynamicObject*> lootitems; // used to store loot items pointers.
        ITextSceneNode* objLabel;

		vector<DynamicObject_Animation> animations; // Animation data
		vector<DynamicObject_material> materials; //Material information data
		vector<DynamicObject_attachment> attachments; //Attachment points data

		bool templateobject;
		bool stunstate; // State to stop moving because the character is hurt
		bool attackdelaystate; // State to stop the attack for a delay after an attack
		int attackresult;

		// delay timers needed
		u32 timerAnimation;
		u32 timerLUA;
		// timers to unlock animation states.
		u32 timerDie;
		u32 timerDespawn;
		u32 timerHurt;
		u32 timerStun;
		// Timer to delay the sound (do it's not repeating too fast)
		u32 lastTime;
		u32 timer_display;


		// Timer to delay the attack
		u32 timer_attackdelay;

		vector<bool> soundActivated;
		vector<bool> attackActivated;
		bool error;

		//Used to calculate a ratio when the node is scaled.
		core::vector3df originalscale;

		//Store the filename for the item thumbnail (retrieved from the template)
		core::stringc thumbnail;
		core::stringw description; //description of the object (Mostly used for loot items), could be used to describe characters.

		ISound * soundfx;


		lua_State *ls;
	

};

#endif // DYNAMICOBJECT_H
