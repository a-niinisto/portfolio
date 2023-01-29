import React from 'react'
import '@testing-library/jest-dom/extend-expect'
import { render, screen } from '@testing-library/react'
import userEvent from '@testing-library/user-event'
import Blog from './Blog'

describe('blog', () => {
    let container
    const mockHandler = jest.fn()
    beforeEach(() => {
        const blog = {
            title: 'testi',
            author: 'joku',
            url: 'jotain',
            likes: 0,
            user: {
                name: 'joku1',
                username: 'joku1'
            }
        }
        const user = {
            name: 'joku1',
            username: 'joku1'
        }
        container = render(
            <Blog blog={blog} user={user} handleBlogUpdate={mockHandler}></Blog>
          ).container
    })
    test('blog title and author renders but url and likes do not render', () => {
        const element = screen.getByText('testi joku')
        expect(element).toBeDefined()
    })

    test('url, likes and user shown if button view is pressed once', async () => {
        {
            const user = userEvent.setup()
            const button = screen.getByText('view')
            await user.click(button)

            const div = container.querySelector('.togglableContent')
             expect(div).not.toHaveStyle('display: none')
        }
    })
    test('if button like is pressed 2 times corresponding eventhandler is called 2 times', async () => {
        const user = userEvent.setup()
        const button = screen.getByText('like')
        await user.click(button)
        await user.click(button)

        expect(mockHandler.mock.calls).toHaveLength(2)
    })
})