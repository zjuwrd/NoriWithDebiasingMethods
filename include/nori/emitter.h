/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#pragma once

#include <nori/object.h>

NORI_NAMESPACE_BEGIN

/**
 * \brief Superclass of all emitters
 */

struct LightQueryRecord
{
    Point3f Light_Sample_point = Point3f(0.f,0.f,0.f);
    Point3f Primitive_point = Point3f(0.f,0.f,0.f);
    Normal3f AreaLight_normal = Point3f(0.f,1.f,0.f);
    
    LightQueryRecord(){}
    LightQueryRecord(Point3f Light_Sample_point,Point3f Primitive_point,Normal3f normal) 
    :Light_Sample_point(Light_Sample_point),Primitive_point(Primitive_point),AreaLight_normal(normal) {} 

    Ray3f shadowRay(){return Ray3f(Light_Sample_point,(Primitive_point-Light_Sample_point).normalized(),Epsilon,(Primitive_point-Light_Sample_point).norm()-Epsilon);}
    Vector3f wi(){return (Light_Sample_point-Primitive_point).normalized();}
};

/// @brief unit for message transport
struct EmitterQueryRecord
{
    public:
    /// @brief construct a EmitterQueryRecord with ref as the only signal
    /// @param ref position of the shading point
    EmitterQueryRecord(Point3f ref):ref(ref){}

    EmitterQueryRecord(Point3f ref_,Point3f p_,Normal3f n_):ref(ref_),p(p_),n(n_){
        wi = (p-ref).normalized();
        dist = (p-ref).norm();
    }

    EmitterQueryRecord() = delete;

    Ray3f shadowRay()const{return Ray3f(ref,wi,Epsilon,dist-Epsilon);}
    



    Point3f ref;//shading point position, the only signal sent to the emitter
    
    /*components below all generated by the emitter*/
    Point3f p;  //light position
    Vector3f wi; //vector pointing from shading point to light source 
    Normal3f n; //normal on the light source at p
    float dist; //distance from shading point to light source
    float pdf;  //propability of the light position being selected

    const Emitter* emitter;


};

struct PhotonRay
{
    Ray3f ray;
    Color3f flux;
    bool success=false;

    PhotonRay() = default;
    PhotonRay(const Ray3f& ray, const Color3f flux):ray(ray), flux(flux){
        success = true;
    }

};


class Emitter : public NoriObject {
protected:
    const Mesh* m_mesh=nullptr;
    enum EmitterType{EMITTER_UNKNOWN_TYPE,EMITTER_AREA} m_type = EMITTER_UNKNOWN_TYPE;
public:

    virtual bool isDelta()const{return false;}

    Emitter(){}
    Emitter(const Mesh* mesh):m_mesh(mesh){}
    Emitter(EmitterType type):m_type(type){}

    void attachMesh(const Mesh* mesh){m_mesh=mesh;}

    virtual Color3f getRadiance()const=0;
    virtual Color3f eval(const EmitterQueryRecord& record)const=0;
    virtual Color3f sample(EmitterQueryRecord & lRec, Sampler* sampler) const=0;
    virtual float pdf(const EmitterQueryRecord &lRec)const = 0;

    virtual float pdf(const Mesh* mesh, const EmitterQueryRecord &eRec) const {return 0.f;}    
    virtual Color3f sample(const Mesh* mesh, EmitterQueryRecord &eRec, Sampler* &sample) const {return 0.f;}
    virtual const Mesh* get_mesh()const{return m_mesh;}
    /**
     * \brief Return the type of object (i.e. Mesh/Emitter/etc.) 
     * provided by this instance
     * */
    EClassType getClassType() const { return EEmitter; }


    virtual PhotonRay ShootPhoton(Sampler* sampler) const {
        return PhotonRay();
    }

    
};

NORI_NAMESPACE_END
