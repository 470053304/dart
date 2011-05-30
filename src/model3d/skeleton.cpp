#include "skeleton.h"

#include "dof.h"
#include "joint.h"
#include "bodynode.h"

#include <cassert>
using namespace std;

namespace model3d {

  Skeleton::Skeleton() {
    mMass = 0;
  }

  Skeleton::~Skeleton(){
    for(int i = 0; i < mJoints.size(); i++) delete mJoints[i];
    mJoints.clear();
    mDofs.clear();
    mTransforms.clear();
    for(int i=0; i<mNodes.size(); i++) delete mNodes[i];
    mNodes.clear();
    mHandles.clear();
  }

  void Skeleton::addHandle(Marker *_h) {
    mHandles.push_back(_h);
    _h->setModelIndex(mHandles.size()-1);
    BodyNode *body = _h->getNode();
    body->addHandle(_h);
  }

  void Skeleton::addNode(BodyNode *b) {
    mNodes.push_back(b);
    b->setModelIndex(mNodes.size()-1);
    addJoint(b->getJointIn());
  }

  void Skeleton::addJoint(Joint *_j) {
    mJoints.push_back(_j);
    _j->setModelIndex(mJoints.size()-1);
  }

  void Skeleton::addDof(Dof *q) {
    mDofs.push_back(q);
    q->setModelIndex(mDofs.size()-1);
    q->setVariable();
  }

  void Skeleton::addTransform(Transformation *t) {
    mTransforms.push_back(t);
    t->setVariable(true);
    t->setModelIndex(mTransforms.size()-1);
    for(int i=0; i<t->getNumDofs(); i++) {
      addDof(t->getDof(i));
    }
  }
  
  void Skeleton::initSkel() {
    mRoot = mNodes[0];
    nDofs = mDofs.size();
    nNodes = mNodes.size();
    nHandles = mHandles.size();

    // calculate mass
    // init the dependsOnDof stucture for each bodylink
    for(int i=0; i<nNodes; i++) {
      mNodes[i]->setSkel(this);
      setDependDofMap(mNodes[i]);
      mNodes.at(i)->init();
      mMass += mNodes[i]->getMass();
    }

    mCurrState = VectorXd::Zero(nDofs);

    for(int i=0; i<nDofs; i++)
      mCurrState[i] = mDofs.at(i)->getValue();
   // for(int i=0; i<nNodes; i++)
   //   mNodes.at(i)->update(mCurrState);
 }

  void Skeleton::setDependDofMap(BodyNode* b){
    // init the map
    b->dependsOnDof = new bool[mDofs.size()];
    memset(b->dependsOnDof, false, mDofs.size()*sizeof(bool));
    // if not the root node, then copy parent's map first
    if(b->getNodeIn()!=NULL){
      // check if map already computed for parent bodylink
      if(b->getNodeIn()->dependsOnDof==NULL){	// map not set for parent
        setDependDofMap(b->getNodeIn());
      }
      // copy the parent map
      memcpy(b->dependsOnDof, b->getNodeIn()->dependsOnDof, mDofs.size()*sizeof(bool));
    }
    // set the self dofs map
    for(int i=0; i<b->getNumDofs(); i++){
      int dofid = b->getDof(i)->getModelIndex();
      b->dependsOnDof[dofid]=true;
    }	
  }

  BodyNode* Skeleton::getNode(const char* const name) {
    const int nNodes = getNumNodes();
    for(int i = 0; i < nNodes; i++){
      BodyNode* node = getNode(i);
      if (strcmp(name, node->getName()) == 0) {
        return node;
      }
    }
    return NULL;
  }

  int Skeleton::getNodeIndex(const char* const name) {
    const int nNodes = getNumNodes();
    for(int i = 0; i < nNodes; i++){
      BodyNode* node = getNode(i);
      if (strcmp(name, node->getName()) == 0) {
        return i;
      }
    }
    return -1;
  }

  VectorXd Skeleton::EvalCOM() {
    const int nNodes = getNumNodes();
    double mass = getMass();
    VectorXd com = VectorXd::Zero(3);

    for(int i = 0; i < nNodes; i++){
      BodyNode* node = getNode(i);
      com += node->getMass() * node->evalCOM();
    }
    com /= mass;
    return com;
  }
  
  void Skeleton::setState(const VectorXd& state) {
    assert(state.size() == nDofs);
    int k=0;
    for(k=0; k<nDofs; k++)
      if(mCurrState[k]!=state[k]) break;
    if(k==nDofs) return;

    mCurrState = state;
    for(int i=0; i<nDofs; i++)
      mDofs.at(i)->setValue(state[i]);
    for(int i=0; i<nNodes; i++)
      mNodes.at(i)->update(mCurrState);
  }

  void Skeleton::setState(const vector<double>& state) {
    assert(state.size()==nDofs);
    int k=0;
    for(k=0; k<nDofs; k++)
      if(mCurrState[k]!=state.at(k)) break;
    if(k==nDofs) return;

    for(int i=0; i<nDofs; i++){
      mDofs.at(i)->setValue(state.at(i));
      mCurrState[k] = state.at(i);
    }
    for(int i=0; i<nNodes; i++)
      mNodes.at(i)->update(mCurrState);
  }

  void Skeleton::setPose(const VectorXd& _pose){
#ifdef DEBUG
	  assert(_pose.size() == nDofs);
#endif
	  for(int i=0; i<nDofs; i++)
		  mDofs[i]->setValue(_pose(i));
  }

  void Skeleton::setPose(const vector<double>& _pose){
#ifdef DEBUG
	  assert(_pose.size() == nDofs);
#endif
	  for(int i=0; i<nDofs; i++)
		  mDofs[i]->setValue(_pose[i]);
  }

  void Skeleton::draw(Renderer::OpenGLRenderInterface* RI, const Vector4d& _color, bool _default) 
  {
		mRoot->draw(RI, _color, _default);
  }
  void Skeleton::drawHandles(Renderer::OpenGLRenderInterface* RI, const Vector4d& _color, bool _default)
  {
		mRoot->drawHandles(RI, _color, _default);
  }


} // namespace model3d
