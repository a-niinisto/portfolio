const blogsRouter = require('express').Router()
const Blog = require('../models/blog')
const User = require('../models/user')
const jwt = require('jsonwebtoken')
const middlewares = require('../utils/middleware')

blogsRouter.get('/', async (request, response) => {
    const blogs = await Blog.find({}).populate('user', {username: 1, name: 1})
    response.json(blogs)

  })


blogsRouter.post('/',middlewares.tokenExtractor, middlewares.userExtractor, async (request, response, next) => {
  let body = request.body
  const user = request.user
  if (body.likes == undefined){
    body.likes = 0
  }
  if (!body.url || !body.title){
    response.status(400).send()
    return
  }

  const blog = new Blog({
    title: body.title,
    author: body.author,
    url: body.url,
    likes: body.likes,
    user: user._id
  })
  
  const result = await blog.save()
  const retunable = await Blog.findById(result._id.toString()).populate('user', {username: 1, name: 1}) //oli pakko tehdä näin kun jostain syystä pelkkä result.populate ei toiminut.
  user.blogs = user.blogs.concat(result._id)
  await user.save()
  response.status(201).json(retunable)
})

blogsRouter.delete('/:id',middlewares.tokenExtractor, middlewares.userExtractor, async (request, response, next) => {
    const user = request.user
    try {
        const blog = await Blog.findById(request.params.id)
        if (blog && blog.user.toString() !== user._id.toString()){
            return response.status(401).json({error: 'cannot delete blogs of other accounts'})
        }
        await Blog.findByIdAndRemove(request.params.id)
        response.status(204).end()
    }
    catch (exception){
        next(exception)
    }
})

blogsRouter.put('/:id', async (request, response, next) => {
    const body = request.body
    const blog = body
    try {
        const updatedBlog = await Blog.findByIdAndUpdate(request.params.id, blog, {new: true, context: 'query'}).populate('user', {username: 1, name: 1})
        if (updatedBlog){
            response.json(updatedBlog)
        }
        else {
            response.status(404).end()
        }
    }
    catch (exception){
        console.log(exception)
        next(exception)
    }
})

module.exports = blogsRouter