#pragma once

#include "Vector.h"
#include "Scope.h"

namespace LibraryDesktop
{
	class Attributed : public Scope
	{
		friend class ActionContainer;
		RTTI_DECLARATIONS(Attributed, Scope)
	public:
		/** @brief					Constructor for the Attributed class.
		*   @return                 Instance of the Attributed class. (However, this is an abstract base class, so the only time this can get called is when a derived class is instantiated.)
		*/
		Attributed();

		/** @brief					Copy constructor for the Attributed class.
		*   @param otherAttribute	Constant reference to a different Attribute instance.
		*   @return                 Instance of the Attributed class from the deep copy of another.
		*/
		Attributed(const Attributed& otherAttribute);

		/** @brief					Move constructor for the Attributed class.
		*   @param otherAttribute	Right-hand reference to a different Attribute instance.
		*   @return                 Instance of the Attributed class from the transferred ownership of another.
		*/
		Attributed(Attributed&& otherAttribute);

		/** @brief					Assignment operator for the Attributed class.
		*   @param otherAttribute	Constant reference to a different Attribute instance.
		*   @return                 Instance of the Attributed class from the deep copy of another.
		*/
		Attributed& operator=(const Attributed& otherAttribute);

		/** @brief					Move assignment operator for the Attributed class.
		*   @param otherAttribute	Right-hand reference to a different Attribute instance.
		*   @return                 Instance of the Attributed class from the transferred ownership of another.
		*/
		Attributed& operator=(Attributed&& otherAttribute);

		/** @brief					Destructor for the Attributed class. (Declared as a pure virtual so instances of the class cannot be made directly.)
		*
		*/
		virtual ~Attributed() = 0;

		/** @brief					Equality operator for the Attributed class. (To be equal, both must have the same attributes.)
		*   @param otherScope		Constant reference to another Attributed instance.
		*   @return                 True if the Attributed instances are the same, and false otherwise.
		*/
		bool Attributed::operator==(const Attributed& otherAttribute) const;

		/** @brief					Inequality operator for the Attributed class. (To be equal, the symbol tables and symbol table entry pointer vectors must be the same.)
		*   @param otherScope		Constant reference to another Attributed instance.
		*   @return                 False if the Attributed instances are the same, and true otherwise.
		*/
		bool Attributed::operator!=(const Attributed& otherAttribute) const;

		/** @brief					Dictates whether or not the attribute with the specified name is prescribed (specified to this class) or not.
		*	@param name				Name of the attribute to check.
		*	@return					True if the attribute exists and is within the list of prescribed attributes, and false otherwise.
		*/
		bool IsPrescribedAttribute(const std::string& name) const;

		/** @brief					Dictates whether or not the attribute with the specified name is auxiliary (specific to the instance of this object) or not.
		*	@param name				Name of the attribute to check.
		*	@return					True if the attribute exists and is not within the list of prescribed attributes, and false otherwise.
		*/
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/** @brief					Dictates whether or not there exists an attribute with the specified name.
		*	@param name				Name of the attribute to check.
		*	@return					True if the attribute exists, and false otherwise.
		*/
		bool IsAttribute(const std::string& name) const;

		/** @brief					Creates and appends an auxiliary attribute to the class if no attribute with the specified name exists, or returns an existing one.
		*	@param name				Name of the attribute.
		*	@return					Datum associated with the attribute.
		*	@exception				An std::exception occurs if an attribute with the specified name exists, but it is prescribed.
		*/
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/** @brief					Gets the index of the beginning of the auxiliary attributes of the class (a.k.a. the end of the prescribed attributes, including "this.")
		*	@return					Index of the location of the first auxiliary attribute.
		*/
		const std::uint32_t AuxiliaryBegin() const;
	protected:
		/** @brief					Creates and appends an internal prescribed attribute (one shared by all instances of the Attributed object) to the list of other prescribed attributes.
		*							(Note that this function should be called only in the constructor to maintain the principle of all prescribed attributes coming before auxiliary attributes.)
		*   @param name				Name of the attribute.
		*	@param initialValue		Default value of the attribute.
		*	@param size				Number of elements the attribute is associated with.
		*	@exception				An std::exception is thrown if there is an existing attribute of the same name, but it is of a different type than the data being used.
		*/
		void AddInternalAttribute(const std::string& name, int initialValue, std::uint32_t size = 1U);
		void AddInternalAttribute(const std::string& name, float initialValue, std::uint32_t size = 1U);
		void AddInternalAttribute(const std::string& name, const glm::vec4& initialValue, std::uint32_t size = 1U);
		void AddInternalAttribute(const std::string& name, const glm::mat4& initialValue, std::uint32_t size = 1U);
		void AddNestedScopeAttribute(const std::string& name, std::uint32_t size = 0U);
		void AddInternalAttribute(const std::string& name, const std::string& initialValue, std::uint32_t size = 1U);
		void AddInternalAttribute(const std::string& name, RTTI* initialValue, std::uint32_t size = 1U);

		/** @brief					Creates and appends an external prescribed attribute (one shared by all instances of the Attributed object) to the list of other prescribed attributes.
		*							(Note that this function should be called only in the constructor to maintain the principle of all prescribed attributes coming before auxiliary attributes.)
		*   @param name				Name of the attribute.
		*	@param size				Number of elements the attribute is associated with.
		*	@param externalStorage	Address of external storage.
		*	@exception				An std::exception is thrown if there is an existing attribute of the same name, but it is of a different type than the data being used.
		*/
		void AddExternalAttribute(const std::string& name, std::uint32_t size, int* externalValues);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, float* externalValues);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, glm::vec4* externalValues);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, glm::mat4* externalValues);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, std::string* externalValues);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, RTTI** externalValues);

		/** @brief					When external attributes or copied or moved, this will fix up the pointers such that the addresses correspond to this particular attribute's external storage containers.
		*   @param name				Name of the attribute.
		*	@param size				Number of elements the attribute is associated with.
		*	@param externalStorage	Address of external storage.
		*	@exception				An std::exception is thrown if there is an existing attribute of the same name, but it is of a different type than the data being used.
		*/
		void UpdateExternalStorage(const std::string& name, std::uint32_t size, int* externalValues);
		void UpdateExternalStorage(const std::string& name, std::uint32_t size, float* externalValues);
		void UpdateExternalStorage(const std::string& name, std::uint32_t size, glm::vec4* externalValues);
		void UpdateExternalStorage(const std::string& name, std::uint32_t size, glm::mat4* externalValues);
		void UpdateExternalStorage(const std::string& name, std::uint32_t size, std::string* externalValues);
		void UpdateExternalStorage(const std::string& name, std::uint32_t size, RTTI** externalValues);

		void ReceiveAuxiliaryAttributes(Attributed& otherAttributed);

		/** @brief					Provides another Attributed instance with any of this Attributed's auxiliary attributes.
		*   @param otherAttributed  AttributedReaction instance to store the attributes in.
		*/
		void DistributeAuxiliaryAttributes(Attributed& otherAttributed);

		/** @brief					Name of the container of Action objects.
		*
		*/
		static const std::string ACTION_CONTAINER;

		/** @brief					Name of the container of Reaction objects.
		*
		*/
		static const std::string REACTION_CONTAINER;

		/** @brief					Name of the container of Entity objects.
		*
		*/
		static const std::string ENTITY_CONTAINER;

		/** @brief					Name of the prescribed Sector attribute.
		*
		*/
		static const std::string SECTOR_CONTAINER;
	private:
		/** @brief					Number of prescribed signatures.
		*
		*/
		std::uint32_t mNumPrescribedSigs;

		/** @brief					Attempts to retrieve a prescribed attribute with the specified name.
		*   @param name				Name of the attribute to search for.
		*   @return                 The address of the signature with the specified name, or nullptr if it doesn't exist.
		*/
		Datum* GetSignature(const std::string& name, std::uint32_t& sigIndex) const;

		/** @brief					Creates a new Datum to be added as a prescribed signature.
		*   @param name				Name of the attribute to append.
		*   @return                 A reference to the newly-created Datum.
		*/
		Datum& AppendDatumForAttribute(const std::string& name);
	};
}

