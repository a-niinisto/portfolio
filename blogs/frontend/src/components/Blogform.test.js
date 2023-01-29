import React from 'react'
import '@testing-library/jest-dom/extend-expect'
import { render, screen } from '@testing-library/react'
import userEvent from '@testing-library/user-event'
import Blogform from './Blogform'

describe('blogform test', () => {
    const mockHandler = jest.fn()
    beforeEach(() => {
        render(
            <Blogform handleBlogAdd={mockHandler}></Blogform>
          )
    })
    test('callback function is called with proper information when blog is created', async () => {
        const newBlog = {
            title: 'testi1',
            author: 'testi2',
            url: 'testi3'
        }

        const user = userEvent.setup()
        const sendButton = screen.getByText('create')
        const inputs = screen.getAllByRole('textbox')

        await user.type(inputs[0], newBlog.title)
        await user.type(inputs[1], newBlog.author)
        await user.type(inputs[2], newBlog.url)
        await user.click(sendButton)

        expect(mockHandler.mock.calls).toHaveLength(1)

        expect(mockHandler.mock.calls[0][0]).toEqual(newBlog)
    })

    })