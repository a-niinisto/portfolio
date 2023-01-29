import { useState, useEffect, useRef } from 'react'
import Blog from './components/Blog'
import blogService from './services/blogs'
import loginService from './services/login'
import User from './components/User'
import Blogform from './components/Blogform'
import Togglable from './components/Togglable'
const Notification = ({ message }) => {
  if (message === null){
    return null
  }
  return (
    <div className={message.class}>{message.msg}</div>
  )
}

const Loginform = (props) => (
  <form onSubmit={props.handleLogin}>
          <div>
            username
              <input
              type="text"
              value={props.username}
              name="Username"
              onChange={({ target }) => props.setUsername(target.value)}
            />
          </div>
          <div>
            password
              <input
              type="password"
              value={props.password}
              name="Password"
              onChange={({ target }) => props.setPassword(target.value)}
            />
          </div>
          <button type="submit">login</button>
        </form>
  )



const App = () => {
  const [blogs, setBlogs] = useState([])
  const [user, setUser] = useState(null)
  const [message, setMessage] = useState(null)

  //for login form
  const [username, setUsername] = useState('')
  const [password, setPassword] = useState('')


  //for blogform
  const blogformRef = useRef()

  const handleLogin = async (event) => {
    event.preventDefault()

    try{
      const user = await loginService.login({ username, password })

      window.localStorage.setItem('loggedBlogappUser', JSON.stringify(user))
      blogService.setToken(user.token)
      setUser(user)
      setUsername('')
      setPassword('')
      setMessage({ msg: `logged in as ${user.name}`, class: 'notification' })
      setTimeout(() => {
        setMessage(null)
      }, 5000)

    }
    catch (response){
      setMessage({ msg: 'wrong username or password', class: 'error' })
      setTimeout(() => {
        setMessage(null)
      }, 5000)
    }
  }

  const handleLogout = (event) => {
    event.preventDefault()
    window.localStorage.removeItem('loggedBlogappUser')
    setUser(null)
    blogService.setToken(null)
  }

  const handleBlogAdd = async (newBlog) => {
    try {
      const response = await blogService.create(newBlog)
      setBlogs(blogs.concat(response))
      setMessage({ msg: `a new blog ${response.title} by ${response.author} added`, class: 'notification' })
      setTimeout(() => {
        setMessage(null)
      }, 5000)
      blogformRef.current()
    }
    catch( response) {
      setMessage({ msg: response.response.data.error, class: 'error' })
      setTimeout(() => {
        setMessage(null)
      }, 5000)
    }
  }
  const handleBlogUpdate = async (newBlog) => {
    try {
      const response = await blogService.update(newBlog)
      setBlogs(blogs.map(blog => {return(
        blog.id === response.id ? response : blog)
      }))
    }
    catch (response){
      setMessage({ msg: response.response.data.error, class: 'error' })
      setTimeout(() => {
        setMessage(null)
      }, 5000)
    }
    }
  const handleBlogRemove = async (blogToRemove) => {
    if (window.confirm(`Delete ${blogToRemove.title} by ${blogToRemove.author} ?`)){
    try {
      await blogService.remove(blogToRemove)
      setBlogs(blogs.filter((blog) => {
        return(blogToRemove.id !== blog.id)
      }))
      setMessage({ msg: `blog ${blogToRemove.title} by ${blogToRemove.author} deleted`, class: 'notification' })
      setTimeout(() => {
        setMessage(null)
      }, 5000)
    }
    catch (response){
      setMessage({ msg: response.response.data.error, class: 'error' })
      setTimeout(() => {
        setMessage(null)
      }, 5000)
    }}
  }

  const filteredBlogs = (listOfBlogs) => {
    const returnable =  listOfBlogs.sort((blog1, blog2) => {return(blog2.likes - blog1.likes )})
    return (returnable)
  }

  useEffect(() => {
    blogService.getAll().then(blogs =>
      setBlogs( blogs )
    )
  }, [])

  useEffect(() => {
    const loggedUserJson = window.localStorage.getItem('loggedBlogappUser')
    if (loggedUserJson){
      const user = JSON.parse(loggedUserJson)
      setUser(user)
      blogService.setToken(user.token)
    }
  }, [])

    if (user === null){
      return (
        <div>
          <h1>Login to application</h1>
          <Notification message={message}></Notification>
          <Loginform username={username} password={password} setPassword={setPassword} setUsername={setUsername} handleLogin={handleLogin}></Loginform>
        </div>
      )
    }

    return (
    <div>
      <h2>blogs</h2>
      <Notification message={message}></Notification>
      <User user={user} logout={handleLogout}></User>
      <br/>
      <Togglable buttonLabel='create new blog' ref={blogformRef}>
        <Blogform handleBlogAdd={handleBlogAdd}></Blogform>
      </Togglable>
      {filteredBlogs(blogs).map(blog =>
        <Blog key={blog.id} blog={blog} handleBlogUpdate={handleBlogUpdate} handleBlogRemove = {handleBlogRemove} user={user}/>
      )}
    </div>
  )
}

export default App
