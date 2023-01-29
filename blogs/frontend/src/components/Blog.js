import { useState } from 'react'
const Blog = ({ blog, handleBlogUpdate, handleBlogRemove, user }) => {

  const [large, setLarge] = useState(false)
  const showWhenLarge = { display: large ? '' : 'none' }
  const showRemove = { display: user.username === blog.user.username ? '' : 'none' }
  const blogStyle = {
    paddingTop: 10,
    paddingLeft: 2,
    border: 'solid',
    borderWidth: 1,
    marginBottom: 5
  }
  const handleLike = (event) => {
    event.preventDefault()
    const newBlog = {
      id: blog.id,
      user: blog.user.id,
      likes: blog.likes + 1,
      author: blog.author,
      title: blog.title,
      url: blog.url
    }
    handleBlogUpdate(newBlog)
  }

  const handleRemove = (event) => {
    event.preventDefault()
    handleBlogRemove (blog)
  }
  return(
  <div style={blogStyle} className='blog'>
  <div>
    {blog.title} {blog.author}
    <button onClick={() => {setLarge(!large)}}>{large ? 'hide' : 'view'}</button>
  </div>
  <div style={showWhenLarge} className='togglableContent'>
    <div>
      {blog.url}
    </div>
    <div>
      likes {blog.likes}
      <button onClick={handleLike}>like</button>
    </div>
    <div>
      {blog.user.name}
    </div>
    <button style = {showRemove} onClick={handleRemove}>remove</button>
  </div>
  </div>
)}

export default Blog