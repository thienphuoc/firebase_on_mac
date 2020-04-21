/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#define USER_REALTIME_VARIABLE "realtime_variables"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    firebaseInit();
    
    retrieveData([=](bool i_result)
    {
        // if retrieve success we will add helloworld.png image to center of the screen.
        
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            
            auto sprite = Sprite::create("HelloWorld.png");
          
            sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

            this->addChild(sprite, 0);
            
        });
        
    });
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        this->addChild(label, 1);
    }


    return true;
}

void HelloWorld::firebaseInit()
{
    auto app = ::firebase::App::GetInstance();
    
    if (!app)
    {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
       app = ::firebase::App::Create(::firebase::AppOptions(), cocos2d::JniHelper::getEnv(), cocos2d::JniHelper::getActivity());
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
       app = ::firebase::App::Create(::firebase::AppOptions());
    #else
        auto opt_string = cocos2d::FileUtils::getInstance()->getStringFromFile("google-services.json");
       auto options = ::firebase::AppOptions::LoadFromJsonConfig(opt_string.c_str());
       app = ::firebase::App::Create(* options);
    #endif
    }
    
    database = firebase::database::Database::GetInstance(app);
    
}
void HelloWorld::retrieveData(std::function<void(bool success)> callback)
{
    const int k_userID = 8899;

    const std::string k_path = cocos2d::StringUtils::format("users/%d/%s/", k_userID, USER_REALTIME_VARIABLE);//currently , on my database doesnt have a user 8899.
    
    firebase::database::DatabaseReference dbref = database->GetReference();
    
    auto result = dbref.Child(k_path).GetValue();//The query takes a long time (1 to 3 minutes) to return a response.if the app sandbox in the signning & capacities tab added.
        
    result.OnCompletion([callback](const firebase::Future<firebase::database::DataSnapshot>& result_data) {
        
        CCLOG("onCompletion callback");//(1 to 3 minutes)
        
        if (result_data.status() != firebase::kFutureStatusComplete)
        {
            CCLOG("ERROR: GetValue() returned an invalid result.");

            // Handle the error...
            if (callback)
            {
                callback(false);
            }
        }
        else if (result_data.error() != firebase::database::kErrorNone)
        {
            CCLOG("ERROR: GetValue() returned error %d: %s", result_data.error(), result_data.error_message());

            // Handle the error...
            if (callback)
            {
                callback(false);
            }
        }
        else
        {
            CCLOG("SUCCESS:");
            
            //auto snapshot = result_data.result();

            if (callback)
            {
                callback(true);
            }
        }
        
    });
    
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
