#ifndef RESOURCE_HOLDER_H_INCLUDED
#define RESOURCE_HOLDER_H_INCLUDED

#include<initializer_list>
#include<map>
#include<utility>
#include<vector>
#include<boost/optional/optional.hpp>

namespace feudal_wars
{
    template<class identifier_type, class resource_type> class resource_holder
    {
        std::map<identifier_type, resource_type> resources;

        using resource_holder_iterator=typename std::map<identifier_type, resource_type>::const_iterator;
        using resource_holder_reverse_iterator=typename std::map<identifier_type, resource_type>::const_reverse_iterator;

        public:
            resource_holder()=default;
            resource_holder(const std::initializer_list<typename std::map<identifier_type, resource_type>::value_type>&);
            resource_holder(std::initializer_list<typename std::map<identifier_type, resource_type>::value_type>&&);
            resource_holder(const std::map<identifier_type, resource_type>&);
            resource_holder(std::map<identifier_type, resource_type>&&);

            boost::optional<const resource_type&> operator[](const identifier_type&) const;

            void create(const std::initializer_list<typename std::map<identifier_type, resource_type>::value_type>&);
            void create(std::initializer_list<typename std::map<identifier_type, resource_type>>&&);
            void create(const std::map<identifier_type, resource_type>&);
            void create(std::map<identifier_type, resource_type>&&);

            const std::map<identifier_type, resource_type>& get_resources() const;

            resource_holder_iterator begin() const;
            resource_holder_iterator end() const;
            resource_holder_reverse_iterator rbegin() const;
            resource_holder_reverse_iterator rend() const;
    };

    template<class identifier_type, class resource_type> resource_holder<identifier_type, resource_type>::resource_holder(const std::initializer_list<typename std::map<identifier_type, resource_type>::value_type>& incoming_initializer_list):resources(incoming_initializer_list){}

    template<class identifier_type, class resource_type> resource_holder<identifier_type, resource_type>::resource_holder(std::initializer_list<typename std::map<identifier_type, resource_type>::value_type>&& incoming_initializer_list):resources(std::move(incoming_initializer_list)){}

    template<class identifier_type, class resource_type> resource_holder<identifier_type, resource_type>::resource_holder(const std::map<identifier_type, resource_type>& incoming_resources):resources(incoming_resources){}

    template<class identifier_type, class resource_type> resource_holder<identifier_type, resource_type>::resource_holder(std::map<identifier_type, resource_type>&& incoming_resources):resources(std::move(incoming_resources)){}

    template<class identifier_type, class resource_type> boost::optional<const resource_type&> resource_holder<identifier_type, resource_type>::operator[](const identifier_type& incoming_identifier_type) const
    {
        const typename std::map<identifier_type, resource_type>::const_iterator resources_finish(resources.cend());
        const typename std::map<identifier_type, resource_type>::const_iterator resources_iterator(resources.find(incoming_identifier_type));
        const bool resource_found(resources_iterator!=resources_finish);
        if(!resource_found)
        {
            return boost::none;
        }

        const resource_type& resource_data(resources_iterator->second);
        const boost::optional<const resource_type&> optional_resource_data(resource_data);

        return optional_resource_data;
    }

    template<class identifier_type, class resource_type> void resource_holder<identifier_type, resource_type>::create(const std::initializer_list<typename std::map<identifier_type, resource_type>::value_type>& incoming_initializer_list)
    {
        resources=incoming_initializer_list;
    }

    template<class identifier_type, class resource_type> void resource_holder<identifier_type, resource_type>::create(std::initializer_list<typename std::map<identifier_type, resource_type>>&& incoming_initializer_list)
    {
        resources=std::move(incoming_initializer_list);
    }

    template<class identifier_type, class resource_type> void resource_holder<identifier_type, resource_type>::create(const std::map<identifier_type, resource_type>& incoming_resources)
    {
        resources=incoming_resources;
    }

    template<class identifier_type, class resource_type> void resource_holder<identifier_type, resource_type>::create(std::map<identifier_type, resource_type>&& incoming_resources)
    {
        resources=std::move(incoming_resources);
    }

    template<class identifier_type, class resource_type> const std::map<identifier_type, resource_type>& resource_holder<identifier_type, resource_type>::get_resources() const
    {
        return resources;
    }

    template<class identifier_type, class resource_type> typename resource_holder<identifier_type, resource_type>::resource_holder_iterator resource_holder<identifier_type, resource_type>::begin() const
    {
        const resource_holder_iterator holder_begin(resources.cbegin());

        return holder_begin;
    }

    template<class identifier_type, class resource_type> typename resource_holder<identifier_type, resource_type>::resource_holder_iterator resource_holder<identifier_type, resource_type>::end() const
    {
        const resource_holder_iterator holder_end(resources.cend());

        return holder_end;
    }

    template<class identifier_type, class resource_type> typename resource_holder<identifier_type, resource_type>::resource_holder_reverse_iterator resource_holder<identifier_type, resource_type>::rbegin() const
    {
        const resource_holder_reverse_iterator holder_reverse_begin(resources.crbegin());

        return holder_reverse_begin;
    }

    template<class identifier_type, class resource_type> typename resource_holder<identifier_type, resource_type>::resource_holder_reverse_iterator resource_holder<identifier_type, resource_type>::rend() const
    {
        const resource_holder_reverse_iterator holder_reverse_end(resources.crend());

        return holder_reverse_end;
    }
}

#endif
