#ifndef TEXTURE_SHEET_H_INCLUDED
#define TEXTURE_SHEET_H_INCLUDED

#include<cstddef>
#include<utility>
#include<vector>
#include<boost/optional/optional.hpp>
#include<SFML/Graphics/Image.hpp>
#include<SFML/Graphics/Rect.hpp>
#include<SFML/Graphics/Texture.hpp>
#include<SFML/System/Vector2.hpp>

namespace feudal_wars
{
    class texture_sheet
    {
        std::pair<std::size_t, std::size_t> frame_dimensions;
        std::pair<std::size_t, std::size_t> sheet_dimensions;
        std::vector<sf::Texture> textures;

        public:
            using texture_sheet_iterator=std::vector<sf::Texture>::const_iterator;
            using texture_sheet_reverse_iterator=std::vector<sf::Texture>::const_reverse_iterator;

            texture_sheet()=default;
            texture_sheet(const std::pair<std::size_t, std::size_t>&, const std::vector<unsigned char>&);
            texture_sheet(const std::pair<std::size_t, std::size_t>&, const sf::Image&);

            boost::optional<const sf::Texture&> operator[](const std::size_t) const;

            bool create(const std::pair<std::size_t, std::size_t>&, const std::vector<unsigned char>&);
            bool create(const std::pair<std::size_t, std::size_t>&, const sf::Image&);
            void clear();

            const std::pair<std::size_t, std::size_t>& get_frame_dimensions() const;
            const std::pair<std::size_t, std::size_t>& get_sheet_dimensions() const;
            const std::vector<sf::Texture>& get_textures() const;
            std::size_t get_frame_count() const;

            texture_sheet_iterator begin() const;
            texture_sheet_iterator end() const;
            texture_sheet_reverse_iterator rbegin() const;
            texture_sheet_reverse_iterator rend() const;
    };
}

#endif
