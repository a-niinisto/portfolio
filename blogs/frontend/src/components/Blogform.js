import { useState } from 'react'
import PropTypes from 'prop-types'

const Blogform = (props) => {
    Blogform.propTypes = {
        handleBlogAdd: PropTypes.func.isRequired
    }
    const [title, setTitle] = useState('')
    const [author, setAuthor] = useState('')
    const [url, setUrl] = useState('')
    const addBlog = (event) => {
        event.preventDefault()
        const newBlog ={
            title, author, url
        }
        props.handleBlogAdd(newBlog)
        setTitle('')
        setAuthor('')
        setUrl('')
    }
    return (
    <form onSubmit={addBlog} id='blogForm'>
      <h2>Create new</h2>
      <div>
        title:
        <input
                type="text"
                value={title}
                name="Title"
                onChange={({ target }) => setTitle(target.value)}
              />
      </div>
      <div>
        author:
        <input
                type="text"
                value={author}
                name="Author"
                onChange={({ target }) => setAuthor(target.value)}
              />
      </div>
      <div>
        url:
        <input
                type="text"
                value={url}
                name="Url"
                onChange={({ target }) => setUrl(target.value)}
              />
      </div>
      <button type="submit">create</button>
    </form>
  )}

export default Blogform