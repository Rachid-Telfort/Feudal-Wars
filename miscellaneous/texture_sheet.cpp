#include<texture_sheet.h>

namespace feudal_wars
{
    texture_sheet::texture_sheet(const std::pair<std::size_t, std::size_t>& incoming_frame_dimensions, const std::vector<unsigned char>& incoming_data)
    {
        create(incoming_frame_dimensions, incoming_data);
    }

    texture_sheet::texture_sheet(const std::pair<std::size_t, std::size_t>& incoming_frame_dimensions, const sf::Image& incoming_image)
    {
        create(incoming_frame_dimensions, incoming_image);
    }

    boost::optional<const sf::Texture&> texture_sheet::operator[](const std::size_t incoming_frame_index) const
    {
        const std::vector<sf::Texture>::size_type frame_index(static_cast<std::vector<sf::Texture>::size_type>(incoming_frame_index));
        const std::vector<sf::Texture>::size_type textures_size(textures.size());
        const bool frame_index_within_bounds(frame_index<textures_size);
        if(!frame_index_within_bounds)
        {
            return boost::none;
        }

        const sf::Texture& texture(textures[frame_index]);
        const boost::optional<const sf::Texture&> optional_texture(texture);

        return optional_texture;
    }

    bool texture_sheet::create(const std::pair<std::size_t, std::size_t>& incoming_frame_dimensions, const std::vector<unsigned char>& incoming_data)
    {
        const std::size_t incoming_data_size(static_cast<std::size_t>(incoming_data.size()));
        const unsigned char* const incoming_data_start(incoming_data.data());

        sf::Image image;
        image.loadFromMemory(incoming_data_start, incoming_data_size);

        const bool texture_sheet_created(create(incoming_frame_dimensions, image));

        return texture_sheet_created;
    }

    bool texture_sheet::create(const std::pair<std::size_t, std::size_t>& incoming_frame_dimensions, const sf::Image& incoming_image)
    {
        clear();

        const std::size_t frame_height(incoming_frame_dimensions.second);
        const std::size_t frame_width(incoming_frame_dimensions.first);

        bool frame_dimensions_valid(static_cast<bool>(frame_height));
        if(!frame_dimensions_valid)
        {
            return frame_dimensions_valid;
        }

        frame_dimensions_valid=static_cast<bool>(frame_width);
        if(!frame_dimensions_valid)
        {
            return frame_dimensions_valid;
        }

        const sf::Vector2u image_size(incoming_image.getSize());
        const std::size_t image_height(static_cast<std::size_t>(image_size.y));
        const std::size_t image_width(static_cast<std::size_t>(image_size.x));
        const std::size_t frame_height_count(image_height/frame_height);
        const std::size_t frame_width_count(image_width/frame_width);

        frame_dimensions_valid=static_cast<bool>(frame_height_count);
        if(!frame_dimensions_valid)
        {
            return frame_dimensions_valid;
        }

        frame_dimensions_valid=static_cast<bool>(frame_width_count);
        if(!frame_dimensions_valid)
        {
            return frame_dimensions_valid;
        }

        std::size_t frame_horizontal_start_coordinate(0u);
        std::size_t frame_vertical_start_coordinate(0u);

        sf::IntRect frame_rectangle;
        sf::Texture frame_texture;

        frame_rectangle.height=static_cast<int>(frame_height);
        frame_rectangle.width=static_cast<int>(frame_width);
        for(std::size_t vertical_frame_index(0u); vertical_frame_index<frame_height_count; ++vertical_frame_index)
        {
            for(std::size_t horizontal_frame_index(0u); horizontal_frame_index<frame_width_count; ++horizontal_frame_index)
            {
                frame_horizontal_start_coordinate=horizontal_frame_index*frame_width;
                frame_vertical_start_coordinate=vertical_frame_index*frame_height;
                frame_rectangle.left=static_cast<int>(frame_horizontal_start_coordinate);
                frame_rectangle.top=static_cast<int>(frame_vertical_start_coordinate);
                frame_dimensions_valid=frame_texture.loadFromImage(incoming_image, frame_rectangle);
                if(!frame_dimensions_valid)
                {
                    clear();

                    return frame_dimensions_valid;
                }

                textures.push_back(frame_texture);
            }
        }

        frame_dimensions=incoming_frame_dimensions;
        sheet_dimensions=std::make_pair(image_width, image_height);

        return frame_dimensions_valid;
    }

    void texture_sheet::clear()
    {
        std::size_t& frame_dimensions_height(frame_dimensions.second);
        std::size_t& frame_dimensions_width(frame_dimensions.first);
        std::size_t& sheet_dimensions_height(sheet_dimensions.second);
        std::size_t& sheet_dimensions_width(sheet_dimensions.first);

        frame_dimensions_height=0u;
        frame_dimensions_width=0u;
        sheet_dimensions_height=0u;
        sheet_dimensions_width=0u;
        textures.clear();
    }

    const std::pair<std::size_t, std::size_t>& texture_sheet::get_frame_dimensions() const
    {
        return frame_dimensions;
    }

    const std::pair<std::size_t, std::size_t>& texture_sheet::get_sheet_dimensions() const
    {
        return sheet_dimensions;
    }

    const std::vector<sf::Texture>& texture_sheet::get_textures() const
    {
        return textures;
    }

    std::size_t texture_sheet::get_frame_count() const
    {
        const std::vector<sf::Texture>::size_type textures_size(textures.size());
        const std::size_t frame_count(static_cast<std::size_t>(textures_size));

        return frame_count;
    }

    texture_sheet::texture_sheet_iterator texture_sheet::begin() const
    {
        const texture_sheet::texture_sheet_iterator sheet_begin(textures.cbegin());

        return sheet_begin;
    }

    texture_sheet::texture_sheet_iterator texture_sheet::end() const
    {
        const texture_sheet::texture_sheet_iterator sheet_end(textures.cend());

        return sheet_end;
    }

    texture_sheet::texture_sheet_reverse_iterator texture_sheet::rbegin() const
    {
        const texture_sheet::texture_sheet_reverse_iterator sheet_reverse_begin(textures.crbegin());

        return sheet_reverse_begin;
    }

    texture_sheet::texture_sheet_reverse_iterator texture_sheet::rend() const
    {
        const texture_sheet::texture_sheet_reverse_iterator sheet_reverse_end(textures.crend());

        return sheet_reverse_end;
    }
}
