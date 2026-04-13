#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/ModelInterfaces.hpp>

class ModelListener
{
public:
   ModelListener()
      : model(0) {}
    
   virtual ~ModelListener() {}

   void bind(IModel* m)
   {
      model = m;
   }

   virtual void onBrewingSessionUpdated(const BrewingSession& session)
   {
      (void)session;
   }

   virtual void onBrewingSessionCompleted()
   {
   }
protected:
   IModel* model;
};

#endif // MODELLISTENER_HPP
