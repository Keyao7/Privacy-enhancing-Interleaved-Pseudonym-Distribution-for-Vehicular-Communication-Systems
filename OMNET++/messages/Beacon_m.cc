//
// Generated file, do not edit! Created by opp_msgtool 6.0 from veins/modules/application/Thesis_src/messages/Beacon.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "Beacon_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace veins {

Register_Class(Beacon)

Beacon::Beacon(const char *name, short kind) : ::veins::DemoSafetyMessage(name, kind)
{
}

Beacon::Beacon(const Beacon& other) : ::veins::DemoSafetyMessage(other)
{
    copy(other);
}

Beacon::~Beacon()
{
}

Beacon& Beacon::operator=(const Beacon& other)
{
    if (this == &other) return *this;
    ::veins::DemoSafetyMessage::operator=(other);
    copy(other);
    return *this;
}

void Beacon::copy(const Beacon& other)
{
    this->senderAddress = other.senderAddress;
    this->pseudonym = other.pseudonym;
    this->pcaID = other.pcaID;
    this->encrypted = other.encrypted;
    this->senderAngle = other.senderAngle;
    this->timestamp = other.timestamp;
    this->CPN_readyFlag = other.CPN_readyFlag;
    this->laneID = other.laneID;
    this->roadID = other.roadID;
    this->pseudonymChangeFlag = other.pseudonymChangeFlag;
}

void Beacon::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::veins::DemoSafetyMessage::parsimPack(b);
    doParsimPacking(b,this->senderAddress);
    doParsimPacking(b,this->pseudonym);
    doParsimPacking(b,this->pcaID);
    doParsimPacking(b,this->encrypted);
    doParsimPacking(b,this->senderAngle);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->CPN_readyFlag);
    doParsimPacking(b,this->laneID);
    doParsimPacking(b,this->roadID);
    doParsimPacking(b,this->pseudonymChangeFlag);
}

void Beacon::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::veins::DemoSafetyMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->senderAddress);
    doParsimUnpacking(b,this->pseudonym);
    doParsimUnpacking(b,this->pcaID);
    doParsimUnpacking(b,this->encrypted);
    doParsimUnpacking(b,this->senderAngle);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->CPN_readyFlag);
    doParsimUnpacking(b,this->laneID);
    doParsimUnpacking(b,this->roadID);
    doParsimUnpacking(b,this->pseudonymChangeFlag);
}

int Beacon::getSenderAddress() const
{
    return this->senderAddress;
}

void Beacon::setSenderAddress(int senderAddress)
{
    this->senderAddress = senderAddress;
}

uint64_t Beacon::getPseudonym() const
{
    return this->pseudonym;
}

void Beacon::setPseudonym(uint64_t pseudonym)
{
    this->pseudonym = pseudonym;
}

uint64_t Beacon::getPcaID() const
{
    return this->pcaID;
}

void Beacon::setPcaID(uint64_t pcaID)
{
    this->pcaID = pcaID;
}

bool Beacon::getEncrypted() const
{
    return this->encrypted;
}

void Beacon::setEncrypted(bool encrypted)
{
    this->encrypted = encrypted;
}

double Beacon::getSenderAngle() const
{
    return this->senderAngle;
}

void Beacon::setSenderAngle(double senderAngle)
{
    this->senderAngle = senderAngle;
}

::omnetpp::simtime_t Beacon::getTimestamp() const
{
    return this->timestamp;
}

void Beacon::setTimestamp(::omnetpp::simtime_t timestamp)
{
    this->timestamp = timestamp;
}

bool Beacon::getCPN_readyFlag() const
{
    return this->CPN_readyFlag;
}

void Beacon::setCPN_readyFlag(bool CPN_readyFlag)
{
    this->CPN_readyFlag = CPN_readyFlag;
}

const char * Beacon::getLaneID() const
{
    return this->laneID.c_str();
}

void Beacon::setLaneID(const char * laneID)
{
    this->laneID = laneID;
}

const char * Beacon::getRoadID() const
{
    return this->roadID.c_str();
}

void Beacon::setRoadID(const char * roadID)
{
    this->roadID = roadID;
}

int Beacon::getPseudonymChangeFlag() const
{
    return this->pseudonymChangeFlag;
}

void Beacon::setPseudonymChangeFlag(int pseudonymChangeFlag)
{
    this->pseudonymChangeFlag = pseudonymChangeFlag;
}

class BeaconDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_senderAddress,
        FIELD_pseudonym,
        FIELD_pcaID,
        FIELD_encrypted,
        FIELD_senderAngle,
        FIELD_timestamp,
        FIELD_CPN_readyFlag,
        FIELD_laneID,
        FIELD_roadID,
        FIELD_pseudonymChangeFlag,
    };
  public:
    BeaconDescriptor();
    virtual ~BeaconDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(BeaconDescriptor)

BeaconDescriptor::BeaconDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(veins::Beacon)), "veins::DemoSafetyMessage")
{
    propertyNames = nullptr;
}

BeaconDescriptor::~BeaconDescriptor()
{
    delete[] propertyNames;
}

bool BeaconDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Beacon *>(obj)!=nullptr;
}

const char **BeaconDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *BeaconDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int BeaconDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 10+base->getFieldCount() : 10;
}

unsigned int BeaconDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_senderAddress
        FD_ISEDITABLE,    // FIELD_pseudonym
        FD_ISEDITABLE,    // FIELD_pcaID
        FD_ISEDITABLE,    // FIELD_encrypted
        FD_ISEDITABLE,    // FIELD_senderAngle
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_CPN_readyFlag
        FD_ISEDITABLE,    // FIELD_laneID
        FD_ISEDITABLE,    // FIELD_roadID
        FD_ISEDITABLE,    // FIELD_pseudonymChangeFlag
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
}

const char *BeaconDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "senderAddress",
        "pseudonym",
        "pcaID",
        "encrypted",
        "senderAngle",
        "timestamp",
        "CPN_readyFlag",
        "laneID",
        "roadID",
        "pseudonymChangeFlag",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int BeaconDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "senderAddress") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "pseudonym") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "pcaID") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "encrypted") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "senderAngle") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "CPN_readyFlag") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "laneID") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "roadID") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "pseudonymChangeFlag") == 0) return baseIndex + 9;
    return base ? base->findField(fieldName) : -1;
}

const char *BeaconDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_senderAddress
        "uint64_t",    // FIELD_pseudonym
        "uint64_t",    // FIELD_pcaID
        "bool",    // FIELD_encrypted
        "double",    // FIELD_senderAngle
        "omnetpp::simtime_t",    // FIELD_timestamp
        "bool",    // FIELD_CPN_readyFlag
        "string",    // FIELD_laneID
        "string",    // FIELD_roadID
        "int",    // FIELD_pseudonymChangeFlag
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
}

const char **BeaconDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BeaconDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BeaconDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    Beacon *pp = omnetpp::fromAnyPtr<Beacon>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void BeaconDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    Beacon *pp = omnetpp::fromAnyPtr<Beacon>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'Beacon'", field);
    }
}

const char *BeaconDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    Beacon *pp = omnetpp::fromAnyPtr<Beacon>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BeaconDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    Beacon *pp = omnetpp::fromAnyPtr<Beacon>(object); (void)pp;
    switch (field) {
        case FIELD_senderAddress: return long2string(pp->getSenderAddress());
        case FIELD_pseudonym: return uint642string(pp->getPseudonym());
        case FIELD_pcaID: return uint642string(pp->getPcaID());
        case FIELD_encrypted: return bool2string(pp->getEncrypted());
        case FIELD_senderAngle: return double2string(pp->getSenderAngle());
        case FIELD_timestamp: return simtime2string(pp->getTimestamp());
        case FIELD_CPN_readyFlag: return bool2string(pp->getCPN_readyFlag());
        case FIELD_laneID: return oppstring2string(pp->getLaneID());
        case FIELD_roadID: return oppstring2string(pp->getRoadID());
        case FIELD_pseudonymChangeFlag: return long2string(pp->getPseudonymChangeFlag());
        default: return "";
    }
}

void BeaconDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Beacon *pp = omnetpp::fromAnyPtr<Beacon>(object); (void)pp;
    switch (field) {
        case FIELD_senderAddress: pp->setSenderAddress(string2long(value)); break;
        case FIELD_pseudonym: pp->setPseudonym(string2uint64(value)); break;
        case FIELD_pcaID: pp->setPcaID(string2uint64(value)); break;
        case FIELD_encrypted: pp->setEncrypted(string2bool(value)); break;
        case FIELD_senderAngle: pp->setSenderAngle(string2double(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2simtime(value)); break;
        case FIELD_CPN_readyFlag: pp->setCPN_readyFlag(string2bool(value)); break;
        case FIELD_laneID: pp->setLaneID((value)); break;
        case FIELD_roadID: pp->setRoadID((value)); break;
        case FIELD_pseudonymChangeFlag: pp->setPseudonymChangeFlag(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Beacon'", field);
    }
}

omnetpp::cValue BeaconDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    Beacon *pp = omnetpp::fromAnyPtr<Beacon>(object); (void)pp;
    switch (field) {
        case FIELD_senderAddress: return pp->getSenderAddress();
        case FIELD_pseudonym: return (omnetpp::intval_t)(pp->getPseudonym());
        case FIELD_pcaID: return (omnetpp::intval_t)(pp->getPcaID());
        case FIELD_encrypted: return pp->getEncrypted();
        case FIELD_senderAngle: return pp->getSenderAngle();
        case FIELD_timestamp: return pp->getTimestamp().dbl();
        case FIELD_CPN_readyFlag: return pp->getCPN_readyFlag();
        case FIELD_laneID: return pp->getLaneID();
        case FIELD_roadID: return pp->getRoadID();
        case FIELD_pseudonymChangeFlag: return pp->getPseudonymChangeFlag();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'Beacon' as cValue -- field index out of range?", field);
    }
}

void BeaconDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Beacon *pp = omnetpp::fromAnyPtr<Beacon>(object); (void)pp;
    switch (field) {
        case FIELD_senderAddress: pp->setSenderAddress(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_pseudonym: pp->setPseudonym(omnetpp::checked_int_cast<uint64_t>(value.intValue())); break;
        case FIELD_pcaID: pp->setPcaID(omnetpp::checked_int_cast<uint64_t>(value.intValue())); break;
        case FIELD_encrypted: pp->setEncrypted(value.boolValue()); break;
        case FIELD_senderAngle: pp->setSenderAngle(value.doubleValue()); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_CPN_readyFlag: pp->setCPN_readyFlag(value.boolValue()); break;
        case FIELD_laneID: pp->setLaneID(value.stringValue()); break;
        case FIELD_roadID: pp->setRoadID(value.stringValue()); break;
        case FIELD_pseudonymChangeFlag: pp->setPseudonymChangeFlag(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Beacon'", field);
    }
}

const char *BeaconDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr BeaconDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    Beacon *pp = omnetpp::fromAnyPtr<Beacon>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void BeaconDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    Beacon *pp = omnetpp::fromAnyPtr<Beacon>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Beacon'", field);
    }
}

}  // namespace veins

namespace omnetpp {

}  // namespace omnetpp

